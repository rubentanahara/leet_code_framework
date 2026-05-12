#pragma once

#include <initializer_list>
#include <optional>
#include <queue>
#include <vector>

namespace lc {

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    explicit ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* n) : val(x), next(n) {}
};

inline ListNode* make_list(std::initializer_list<int> values) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

inline void free_list(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

inline std::vector<int> to_vector(ListNode* head) {
    std::vector<int> out;
    for (ListNode* p = head; p; p = p->next) out.push_back(p->val);
    return out;
}

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* l, TreeNode* r) : val(x), left(l), right(r) {}
};

// Level-order builder. std::nullopt marks a missing node (LeetCode convention).
inline TreeNode* make_tree(std::initializer_list<std::optional<int>> values) {
    auto it = values.begin();
    const auto end = values.end();
    if (it == end || !it->has_value()) return nullptr;

    auto* root = new TreeNode(**it);
    ++it;

    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty() && it != end) {
        TreeNode* node = q.front();
        q.pop();

        if (it != end) {
            if (it->has_value()) {
                node->left = new TreeNode(**it);
                q.push(node->left);
            }
            ++it;
        }
        if (it != end) {
            if (it->has_value()) {
                node->right = new TreeNode(**it);
                q.push(node->right);
            }
            ++it;
        }
    }
    return root;
}

inline void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}

inline bool tree_equal(const TreeNode* a, const TreeNode* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a->val == b->val && tree_equal(a->left, b->left) && tree_equal(a->right, b->right);
}

}  // namespace lc
