#include <memory>
#include <random>
#include <vector>

template<class T, class SubtreeInfo, class Rng_>
class PersistentTreap {

 public:
  template<class Rng>
  PersistentTreap(Rng &&rng)
  : rng_(std::forward<Rng>(rng)),
    roots_history{nullptr},
    present_index(0) {}

  template<class U>
  void push_back(U &&value) {
    set_root(merge(
          root(),
          single_node(std::forward<U>(value))));
  }

  template<class U>
  void push_front(U &&value) {
    set_root(merge(
          single_node(std::forward<U>(value)),
          root()));
  }

  auto cut(size_t i, size_t j) {
    auto [tmp, right_tree] = split(root(), [=](const SubtreeInfo &si){
      return si.size < j;
    });
    auto [left_tree, middle_tree] = split(tmp, [=](const SubtreeInfo &si){
      return si.size < i;
    });
    set_root(merge(left_tree, right_tree));
    return middle_tree;
  }

  template<class U>
  void insert(size_t index, U &&value) {
    auto [left_tree, right_tree] = split(root(), [=](const SubtreeInfo &si){
      return si.size <= index;
    });

    set_root(
     merge(
       merge(
         left_tree,
         single_node(std::forward<U>(value))
       ),
       right_tree
     )
    );
  }

  void undo() {
    if (present_index > 0)
      --present_index;
  }

  void redo() {
    if (present_index+1 < roots_history.size())
      ++present_index;
  }

  template<class CB>
  void traverse(const CB &callback) {
    dfs(root(), callback);
  }

  size_t size() {
    nd_ptr r = root();
    return r ? r->si.size : 0;
  }

 private:
  struct Node;
  using nd_ptr = std::shared_ptr<const Node>;

  Rng_ rng_;

  struct Node {
    template<class U, class LPtr, class RPtr>
    Node(uint32_t priority_, U &&value_, LPtr &&lk_, RPtr &&rk_)
    : priority(priority_),
      value(std::forward<U>(value_)),
      lk(std::forward<LPtr>(lk_)),
      rk(std::forward<RPtr>(rk_)),
      si(value,
         lk ? lk->si : SubtreeInfo(),
         rk ? rk->si : SubtreeInfo()) {}

    uint32_t priority;
    T value;
    nd_ptr lk, rk;
    SubtreeInfo si;
  };

  nd_ptr merge(nd_ptr left_tree, nd_ptr right_tree) {
    if (!left_tree) return right_tree;
    if (!right_tree) return left_tree;

    if (left_tree->priority > right_tree->priority)
      return std::make_shared<const Node>(
          left_tree->priority,
          left_tree->value,
          left_tree->lk,
          merge(left_tree->rk, right_tree));
    else
      return std::make_shared<const Node>(
          right_tree->priority,
          right_tree->value,
          merge(left_tree, right_tree->lk),
          right_tree->rk);
  }

  template<class Cond>
  std::pair<nd_ptr, nd_ptr> split(nd_ptr tree, const Cond &condition, SubtreeInfo accumulated = {}) {
    //TODO: Consider if priority duplication is a problem

    if (!tree) return {nullptr, nullptr};

    SubtreeInfo left_with_root(tree->value, (tree->lk ? tree->lk->si : SubtreeInfo{}), {});
    SubtreeInfo new_accumulated(accumulated, left_with_root);

    if (condition(new_accumulated)) {
      auto [middle_tree, right_tree] = split(tree->rk, condition, new_accumulated);
      return {
        std::make_shared<const Node>(
            tree->priority,
            tree->value,
            tree->lk,
            middle_tree),
        right_tree
      };
    } else {
      auto [left_tree, middle_tree] = split(tree->lk, condition, accumulated);
      return {
        left_tree,
        std::make_shared<const Node>(
            tree->priority,
            tree->value,
            middle_tree,
            tree->rk)
      };
    }
  }

  template<class U>
  nd_ptr single_node(U &&value) {
    return std::make_shared<const Node>(
        rng_(), std::forward<U>(value), nullptr, nullptr);
  }

  nd_ptr root() {
    return roots_history[present_index];
  }

  void set_root(nd_ptr root) {
    roots_history.erase(roots_history.begin()+present_index+1, roots_history.end());

    roots_history.push_back(root);
    ++present_index;
  }

  template<class CB>
  void dfs(nd_ptr cur, const CB &callback) {
    if (!cur) return;
    dfs(cur->lk, callback);
    callback(cur);
    dfs(cur->rk, callback);
  }

  std::vector<nd_ptr> roots_history;
  size_t present_index;
};

struct SubtreeInfo {
  SubtreeInfo() : size(0), lines(0), height(0) {}

  SubtreeInfo(const SubtreeInfo &left_subtree, const SubtreeInfo &right_subtree)
  : size(left_subtree.size + right_subtree.size),
    lines(left_subtree.lines + right_subtree.lines),
    height(std::max(left_subtree.height, right_subtree.height)) {}

  SubtreeInfo(char c, const SubtreeInfo &left_subtree, const SubtreeInfo &right_subtree)
  : size(1 + left_subtree.size + right_subtree.size),
    lines((c == '\n') + left_subtree.lines + right_subtree.lines),
    height(std::max(left_subtree.height, right_subtree.height)+1) {}

  size_t size;
  size_t lines;
  size_t height;
};

#include <chrono>
#include <iostream>
#include <random>
int main() {

  std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  PersistentTreap<char, SubtreeInfo, std::mt19937> t(rng);

  auto print_content = [&]() {
    size_t max_height = 0;
    t.traverse([&](const auto &nd){
      std::cout << nd->value;
      max_height = std::max(max_height, nd->si.height);
    });
    std::cout << '\n';
    std::cout << "max height: " << max_height << "\n\n";
  };

  int n = 1e6;
  std::uniform_int_distribution<char> distr('a', 'z');
  for (int I = 0; I < n; ++I) {
    t.insert(rng()%(t.size()+1), distr(rng));

    std::uniform_int_distribution<size_t> cut_distr(0, t.size());
    size_t i = cut_distr(rng);
    size_t j = cut_distr(rng);
    if (i > j) std::swap(i, j);

    while (j-i > 100)
      std::tie(i, j) = std::make_tuple((3*i+j)/4, (i+3*j)/4);

    if (I % 100 == 0)
      t.cut(i, j);
  }

  print_content();
}
