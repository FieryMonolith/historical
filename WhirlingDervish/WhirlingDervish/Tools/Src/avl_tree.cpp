#include "avl_tree.h"
#include "stack.h"

template <class T, class KEY>
AvlTree<T, KEY>::AvlTree(void) {
  d_allocationType = ALLOCATION_NEW;
  d_root = NULL;
  d_size = 0;

  return;
}

template <class T, class KEY>
AvlTree<T, KEY>::AvlTree(const T data, const KEY key) {
  d_allocationType = ALLOCATION_NEW;
  d_root = NewNode();
  if(d_root == NULL) {
    size = 0;
    return;
  }

  d_root->d_data = data;
  d_root->d_key = key;
  d_size = 1;

  return;
}

template <class T, class KEY>
AvlTree<T, KEY>::~AvlTree(void) {
  if(d_root != NULL) {
    DeleteNode(d_root);
  }

  return;
}

template <class T, class KEY>
bool AvlTree<T, KEY>::Insert(const T data, const KEY key) {
  AvlTreeNode<T, KEY> *currentNode = d_root;
  AvlTreeNode<T, KEY> *newNode = NewNode();
  newNode->d_data = data;
  newNode->d_key = key;

  if(d_root == NULL) {
    d_root = newNode;
    d_size++;
    return true;
  } else if(d_root->d_left == NULL && d_root->d_right == NULL) {
    if(d_root->d_key > key) {
      d_root->d_leftCount++;
      d_root->d_left = newNode;
    } else {
      d_root->d_rightCount++;
      d_root->d_right = newNode;
    }
    newNode->d_parent = d_root;
    d_size++;
    return true;
  }

  while(currentNode->d_left != NULL && currentNode->d_right != NULL) {
    if(currentNode->d_key > key) {
      if(currentNode->d_left != NULL) {
	currentNode = currentNode->d_left;
      } else {
	currentNode->d_left = newNode;
	newNode->d_parent = currentNode;
	break;
      }
    } else {
      if(currentNode->d_right != NULL) {
	currentNode = currentNode->d_right;
      } else {
	currentNode->d_right = newNode;
	newNode->d_parent = currentNode;
	break;
      }
    }
  }

  currentNode = newNode;
  AvlTreeNode<T, KEY> *parent = currentNode->d_parent;
  while(parent != NULL) {
    if(currentNode == parent->d_left) {
      parent->d_leftCount++;
    } else {
      parent->d_rightCount++;
    }

    long difference = parent->d_leftCount - parent->d_rightCount;
    if(difference > 1) {
      RotateRight(parent);
    } else if(difference < -1) {
      RotateLeft(parent);
    }

    currentNode = parent;
    parent = currentNode->d_parent;
  }

  d_size++;
  return true;
}

template <class T, class KEY>
T *AvlTree<T, KEY>::Retrieve(const KEY key) {
  AvlTreeNode<T, KEY> *currentNode = d_root;

  if(d_root == NULL) {
    return NULL;
  } else if(d_root->d_left == NULL && d_root->d_right == NULL) {
    if(d_root->d_key == key) {
      return d_root->d_data;
    }
    return NULL;
  }

  while(currentNode->d_left != NULL && currentNode->d_right != NULL) {
    if(currentNode->d_key == key) {
      return currentNode->d_data;
    } else if(currentNode->d_key > key) {
      if(currentNode->d_left != NULL) {
	currentNode = currentNode->d_left;
      } else {
	return NULL;
      }
    } else {
      if(currentNode->d_right != NULL) {
	currentNode = currentNode->d_right;
      } else {
	return NULL;
      }
    }
  }

  return NULL;
}

template <class T, class KEY>
T *AvlTree<T, KEY>::Delete(const KEY key) {
  AvlTreeNode<T, KEY> *currentNode = d_root;
  T* data = NULL;

  cout << "avl1" << endl;
  if(d_root == NULL) {
    return NULL;
  } else if(d_root->d_left == NULL && d_root->d_right == NULL) {
    if(d_root->d_key == key) {
      data = d_root->d_data;
      DeleteNode(d_root);
      d_root = NULL;
      d_size--;
      return data;
    }
    return NULL;
  }

  cout << "avl2" << endl;
  while(currentNode->d_left != NULL && currentNode->d_right != NULL) {
    if(currentNode->d_key == key) {
      break;
    } else if(currentNode->d_key > key) {
      if(currentNode->d_left != NULL) {
	currentNode = currentNode->d_left;
      } else {
	return NULL;
      }
    } else {
      if(currentNode->d_right != NULL) {
	currentNode = currentNode->d_right;
      } else {
	return NULL;
      }
    }
  }

  cout << "avl3" << endl;
  if(currentNode->d_key != key) {
    return NULL;
  }

  cout << "avl4" << endl;
  AvlTreeNode<T, KEY> *parent = currentNode->d_parent;
  AvlTreeNode<T, KEY> *start;
  if(currentNode->d_right == NULL) {
    if(parent->d_left == currentNode) {
      parent->d_left = currentNode->d_left;
    } else {
      parent->d_right = currentNode->d_left;
    }
  } else if(currentNode->d_left == NULL) {
    if(parent->d_left == currentNode) {
      parent->d_left = currentNode->d_right;
    } else {
      parent->d_right = currentNode->d_right;
    }
  } else {
    AvlTreeNode<T, KEY> *maxNode = currentNode->d_right;
    while(maxNode->d_left != NULL) {
      maxNode = maxNode->d_left;
    }
    if(parent->d_left == currentNode) {
      parent->d_left = maxNode;
    } else {
      parent->d_right = maxNode;
    }

    if(maxNode->d_parent != currentNode) { 
      maxNode->d_parent->d_left = maxNode->d_right;
      maxNode->d_parent->d_leftCount = maxNode->d_rightCount;
      maxNode->d_right = currentNode->d_right;
      maxNode->d_rightCount = currentNode->d_rightCount;
    }
    maxNode->d_left = currentNode->d_left;
    maxNode->d_leftCount = currentNode->d_leftCount;
    parent = maxNode->d_parent;
    if(parent == currentNode) {
      parent = maxNode;
    }
    maxNode->d_parent = currentNode->d_parent;
  }

  cout << "avl5" << endl;
  data = currentNode->d_data;
  DeleteNode(currentNode);

  cout << "avl6" << endl;
  currentNode = parent;
  long diff = currentNode->d_leftCount - currentNode->d_rightCount;
  if(diff > 1) {
    RotateRight(currentNode);
  } else if(diff < -1) {
    RotateLeft(currentNode);
  }
  parent = currentNode->d_parent;
  while(parent != NULL) {
    if(parent->d_left == currentNode) {
      parent->d_leftCount--;
    } else {
      parent->d_rightCount--;
    }
    long difference = parent->d_leftCount - parent->d_rightCount;
    if(difference > 1) {
      RotateRight(parent);
    } else if(difference < -1) {
      RotateLeft(parent);
    }
  }

  cout << "avl7" << endl;
  return data;
}

template <class T, class KEY>
bool AvlTree<T, KEY>::IsEmpty(void) {
  if(size == 0) {
    return true;
  }

  return false;
}

template <class T, class KEY>
void AvlTree<T, KEY>::SetAllocator(int type) {
  d_allocationType = type;

  return;
}

template <class T, class KEY>
int AvlTree<T, KEY>::GetAllocator(void) {
  return d_allocationType;
}

template <class T, class KEY>
void AvlTree<T, KEY>::RemoveSubtree(AvlTreeNode<T, KEY> *root) {
  Stack<AvlTreeNode<T, KEY> *> *removal = NewStack(NULL);
  AvlTreeNode<T, KEY> *currentNode = root;

  while(removal->IsEmpty()) {
    if(currentNode == NULL) {
      currentNode = removal->Pop();
    } else if(currentNode->d_left != NULL || currentNode->d_right != NULL) {
      removal->Push(currentNode);
      removal->Push(currentNode->d_left);
      removal->Push(currentNode->d_right);
      currentNode = currentNode->d_right;
    } else {
      DeleteNode(currentNode);
      currentNode = removal->Pop();
    }
  }

  DeleteStack(removal);

  return;
}

template <class T, class KEY> 
void AvlTree<T, KEY>::RotateLeft(AvlTreeNode<T, KEY> *node) {
  AvlTreeNode<T, KEY> *temp = node->d_right;
  temp->d_parent = node->d_parent;

  node->d_right = temp->d_left;
  node->d_rightCount = temp->d_leftCount;
  node->d_parent = temp;

  temp->d_left = node;
  temp->d_leftCount = node->d_leftCount + node->d_rightCount + 1;

  return;
}

template <class T, class KEY> 
void AvlTree<T, KEY>::RotateRight(AvlTreeNode<T, KEY> *node) {
  AvlTreeNode<T, KEY> *temp = node->d_left;
  temp->d_parent = node->d_parent;

  node->d_left = temp->d_right;
  node->d_leftCount = temp->d_rightCount;
  node->d_parent = temp;

  temp->d_right = node;
  temp->d_rightCount = node->d_leftCount + node->d_rightCount + 1;

  return;
}

template <class T, class KEY> 
AvlTreeNode<T, KEY> *AvlTree<T, KEY>::NewNode(void) {
  AvlTreeNode<T, KEY> *node;
  switch(d_allocationType) {
  case ALLOCATION_MALLOC:
    node = (AvlTreeNode<T, KEY> *)malloc(sizeof(AvlTreeNode<T, KEY>));
    break;
  case ALLOCATION_NEW:
  default:
    node = new AvlTreeNode<T, KEY>();
    break;
  }

  return node;
}

template <class T, class KEY> 
void AvlTree<T, KEY>::DeleteNode(AvlTreeNode<T, KEY> *node) {
  switch(d_allocationType) {
  case ALLOCATION_MALLOC:
    free(node);
    break;
  case ALLOCATION_NEW:
  default:
    delete node;
    break;
  }

  return;
}

template <class T, class KEY> 
Stack<AvlTreeNode<T, KEY> *> *AvlTree<T, KEY>::NewStack(AvlTreeNode<T, KEY> *node) {
  Stack<AvlTreeNode<T, KEY> *> *stack;
  switch(d_allocationType) {
  case ALLOCATION_MALLOC:
    stack = (Stack<AvlTreeNode<T, KEY> *> *)malloc(sizeof(Stack<AvlTreeNode<T, KEY> *>));
    stack->Push(node);
    break;
  case ALLOCATION_NEW:
  default:
    stack = new Stack<AvlTreeNode<T, KEY> *>(node);
    break;
  }

  return stack;
}

template <class T, class KEY> 
void AvlTree<T, KEY>::DeleteStack(Stack<AvlTreeNode<T, KEY> *> *stack) {
  switch(d_allocationType) {
  case ALLOCATION_MALLOC:
    free(stack);
    break;
  case ALLOCATION_NEW:
  default:
    delete stack;
    break;
  }

  return;
}

template <class T, class KEY> 
void AvlTree<T, KEY>::LargestDataKey(T *data, KEY *key) {
  AvlTreeNode<T, KEY> *temp = d_root;
  while(temp->d_right != NULL) {
    temp = temp->d_right;
  }

  *data = temp->d_data;
  *key =  temp->d_key;

  return;
}

template <class T, class KEY> 
void AvlTree<T, KEY>::SmallestDataKey(T *data, KEY *key) {
  AvlTreeNode<T, KEY> *temp = d_root;
  while(temp->d_left != NULL) {
    temp = temp->d_left;
  }

  *data = temp->d_data;
  *key =  temp->d_key;

  return;
}
