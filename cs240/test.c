void tree_insert(tree ** root, tree * ins) {
  assert(root != NULL);
  assert(ins != NULL);
  assert(ins->left_ptr == NULL);
  assert(ins->right_ptr == NULL);

  if ((*root) == NULL) {
    (*root) = ins;
    return;
  }

  if (strcmp(((*root)->name), ins->name) <= 0) {
    tree_insert(&((*root)->left_ptr), ins);
  }
  else if (strcmp(((*root)->name), ins->name) > 0) {
    tree_insert(&((*root)->right_ptr), ins);
  }
  

}
