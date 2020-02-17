#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include "stack.h"

template <class T, class KEY> class AvlTree {
public:
  AvlTree(void);
  AvlTree(const T data, const KEY key);
  ~AvlTree(void);

  bool Insert(T data, KEY key);
  T Retrieve(const KEY key);
  T Delete(const KEY key);
  T Delete(void);
  void LargestDataKey(T *data, KEY *key);
  void SmallestDataKey(T *data, KEY *key);

  // Sets location to the first found instance of that key from the root
  bool SetLocation(KEY key);
  bool SetMax(void);
  bool SetMin(void);
  T Next(void);
  T NextLoop(void);
  T Previous(void);
  T PreviousLoop(void);
  T GetCurrentData(void);
  KEY GetCurrentKey(void);

  bool IsEmpty(void);
  int Size(void);
  void Print(void);

private:
  class Node {
  public:
    Node(void)  { Initialize(); return; }
    ~Node(void) { Initialize(); return; }

    T d_data;
    KEY d_key;
    Node *d_left, *d_right, *d_parent;
    long d_leftMaxDepth, d_rightMaxDepth;

  private:
    void Initialize(void) { d_data=(T)0; d_key=(KEY)0;
    d_left=d_right=d_parent=NULL;
    d_leftMaxDepth=d_rightMaxDepth=0;
    return;
    }
  };

  void PrintTree(Node *temp);
  void Balance(Node *currentNode);

  void RotateLeft(Node *);
  void RotateRight(Node *);
  void RemoveSubtree(Node *);

  Node *d_root;
  Node *d_current;
  Node *d_min, *d_max;
  long d_size;
};

template <class T, class KEY>
AvlTree<T, KEY>::AvlTree(void) {
  d_root = NULL;
  d_size = 0;

  d_min = d_max = d_current = d_root;

  return;
}

template <class T, class KEY>
AvlTree<T, KEY>::AvlTree(const T data, const KEY key) {
  d_root = new Node();
  if(d_root == NULL) {
    size = 0;
    return;
  }

  d_root->d_data = &data;
  d_root->d_key = &key;
  d_size = 1;

  d_min = d_max = d_current = d_root;

  return;
}

template <class T, class KEY>
AvlTree<T, KEY>::~AvlTree(void) {
  if(d_root == NULL) {
    return;
  }

  while(Size()) {
    Delete();
  }
    
  return;
}

template <class T, class KEY>
bool AvlTree<T, KEY>::Insert(T data, KEY key) {
  Node *currentNode = d_root;
  Node *newNode = new Node();
  newNode->d_data = data;
  newNode->d_key = key;

  if(d_root == NULL) {
    d_root = newNode;
    d_size++;
    d_min = d_max = d_current = d_root;
    return true;
  } else if(d_root->d_left == NULL && d_root->d_right == NULL) {
    if((d_root->d_key) > key) {
      d_root->d_leftMaxDepth++;
      d_root->d_left = newNode;
    } else {
      d_root->d_rightMaxDepth++;
      d_root->d_right = newNode;
    }
    newNode->d_parent = d_root;
    d_size++;
    d_current = newNode;
    if(key > d_max->d_key) {
      d_max = newNode;
    } else if(key < d_min->d_key) {
      d_min = newNode;
    }
    return true;
  }

  while(currentNode->d_left != NULL || currentNode->d_right != NULL) {
    if((currentNode->d_key) > key) {
      if(currentNode->d_left != NULL) {
	currentNode = currentNode->d_left;
      } else {
	break;
      }
    } else {
      if(currentNode->d_right != NULL) {
	currentNode = currentNode->d_right;
      } else {
	break;
      }
    }
  }
  if(currentNode->d_key > key) {
    currentNode->d_left = newNode;
    newNode->d_parent = currentNode;
  } else {
    currentNode->d_right = newNode;
    newNode->d_parent = currentNode;
  }

  d_size++;

  d_current = newNode;
  if(key >= d_max->d_key) {
    d_max = newNode;
  } else if(key < d_min->d_key) {
    d_min = newNode;
  }

  int depth = 0;
  currentNode = newNode;
  while(currentNode->d_parent != NULL) {
    if(currentNode == currentNode->d_parent->d_left) {
      if(currentNode->d_parent->d_leftMaxDepth < depth + 1) {
	currentNode->d_parent->d_leftMaxDepth = depth + 1;
      }
    } else {
      if(currentNode->d_parent->d_rightMaxDepth < depth + 1) {
	currentNode->d_parent->d_rightMaxDepth = depth + 1;
      }
    }
    currentNode = currentNode->d_parent;
    depth++;
  }
  Balance(newNode);

  return true;
}

template <class T, class KEY>
T AvlTree<T, KEY>::Retrieve(const KEY key) {
  Node *currentNode = d_root;

  if(d_root == NULL) {
    return (T)0;
  } else if(d_root->d_left == NULL && d_root->d_right == NULL) {
    if((d_root->d_key) == key) {
      d_current = d_root;
      return (d_root->d_data);
    }
    return (T)0;
  }

  while(currentNode->d_left != NULL || currentNode->d_right != NULL) {
    if((currentNode->d_key) == key) {
      d_current = currentNode;
      return (currentNode->d_data);
    } else if((currentNode->d_key) > key) {
      if(currentNode->d_left != NULL) {
	currentNode = currentNode->d_left;
      } else {
	return (T)0;
      }
    } else {
      if(currentNode->d_right != NULL) {
	currentNode = currentNode->d_right;
      } else {
	return (T)0;
      }
    }
  }

  return (T)0;
}

template <class T, class KEY>
T AvlTree<T, KEY>::Delete(void) {
  if(d_current == NULL) {
    return (T)0;
  }

  return Delete(d_current->d_key);
}

template <class T, class KEY>
T AvlTree<T, KEY>::Delete(const KEY key) {
  Node *currentNode = d_root;
  T data;

  if(d_root == NULL) {
    return (T)0;
  } else if(d_root->d_left == NULL && d_root->d_right == NULL) {
    if((d_root->d_key) == key) {
      data = (d_root->d_data);
      delete d_root;
      d_root = NULL;
      d_size--;
      return data;
      d_current = d_max = d_min = NULL;
    }
    return (T)0;
  }

  // Find key
  while(currentNode->d_left != NULL || currentNode->d_right != NULL) {
    if((currentNode->d_key) == key) {
      break;
    } else if((currentNode->d_key) > key) {
      if(currentNode->d_left != NULL) {
	currentNode = currentNode->d_left;
      } else {
	return (T)0;
      }
    } else {
      if(currentNode->d_right != NULL) {
	currentNode = currentNode->d_right;
      } else {
	return (T)0;
      }
    }
  }

  // If you arrive at a leaf node, it will exit the loop above
  if((currentNode->d_key) != key) {
    return (T)0;
  }

  // Find swap node
  Node *swapNode = currentNode;
  if(currentNode->d_right != NULL) {
    swapNode = currentNode->d_right;
    while(swapNode->d_left != NULL) {
      swapNode = swapNode->d_left;
    }
  }

  if(swapNode == currentNode->d_right) {
    swapNode->d_left = currentNode->d_left;
    swapNode->d_leftMaxDepth = currentNode->d_leftMaxDepth;
    if(currentNode->d_left != NULL) {
      currentNode->d_left->d_parent = swapNode;
    }
    swapNode->d_parent = currentNode->d_parent;
    if(currentNode->d_parent != NULL) {
      if(currentNode->d_parent->d_left == currentNode) {
	currentNode->d_parent->d_left = swapNode;
      } else {
	currentNode->d_parent->d_right = swapNode;
      }
    }

    if(currentNode == d_root) {
      d_root = swapNode;
    }
    if(currentNode == d_min) {
      d_min = swapNode;
    }

    if(swapNode->d_right != NULL) {
      Balance(swapNode->d_right);
    }
    data = currentNode->d_data;
    delete currentNode;
    d_current = swapNode;
    d_size--;
    return data;
  } else if(swapNode == currentNode) {
    if(currentNode->d_parent != NULL) {
      if(currentNode->d_parent->d_left == currentNode) {
	currentNode->d_parent->d_left = currentNode->d_left;
	currentNode->d_parent->d_leftMaxDepth = currentNode->d_leftMaxDepth;
      } else {
	currentNode->d_parent->d_right = currentNode->d_left;
	currentNode->d_parent->d_rightMaxDepth = currentNode->d_leftMaxDepth;
      }
    }
    if(currentNode->d_left != NULL) {
      currentNode->d_left->d_parent = currentNode->d_parent;
    }
    // Must be a leaf node to be in this clause and equal to d_min
    if(currentNode == d_min) {
      d_min = currentNode->d_parent;
    }
    if(currentNode == d_max) {
      if(currentNode->d_left == NULL) {
	d_max = currentNode->d_parent;
      } else {
	d_max = currentNode->d_left;
	while(d_max->d_right != NULL) {
	  d_max = d_max->d_right;
	}
      }
    }
      
    if(currentNode->d_parent == NULL) {
      if(currentNode->d_left == NULL) {
	if(d_root == currentNode) {
	  d_root = NULL;
	}
	d_current = NULL;
      } else {
	if(d_root == currentNode) {
	  d_root = currentNode->d_left;
	}
	d_current = currentNode->d_left;
      }
    } else {
      d_current = currentNode->d_parent;
    }
    // No need to assign child's parent, will be done later
  } else if(swapNode != currentNode) {
    // Swap Nodes
    // Set children parents
    if(currentNode->d_left != NULL) {
      currentNode->d_left->d_parent = swapNode;
    }
    if(swapNode->d_left != NULL) {
      swapNode->d_left->d_parent = currentNode;
    }
    currentNode->d_right->d_parent = swapNode;
    if(swapNode->d_right != NULL) {
      swapNode->d_right->d_parent = currentNode;
    }

    // Swap left child pointers
    Node *temp = currentNode->d_left;
    int tempDepth = currentNode->d_leftMaxDepth;
    currentNode->d_left = swapNode->d_left;
    currentNode->d_leftMaxDepth = swapNode->d_leftMaxDepth;
    swapNode->d_left = temp;
    swapNode->d_leftMaxDepth = tempDepth;

    // swap right child pointers
    temp = currentNode->d_right;
    tempDepth = currentNode->d_rightMaxDepth;
    currentNode->d_right = swapNode->d_right;
    currentNode->d_rightMaxDepth = swapNode->d_rightMaxDepth;
    swapNode->d_right = temp;
    swapNode->d_rightMaxDepth = tempDepth;

    // set parent's pointers to swapped children
    if(currentNode->d_parent != NULL) {
      if(currentNode->d_parent->d_left == currentNode) {
	currentNode->d_parent->d_left = swapNode;
      } else {
	currentNode->d_parent->d_left = swapNode;
      }
    }
    if(swapNode->d_parent != NULL) {
      if(swapNode->d_parent->d_left == swapNode) {
	swapNode->d_parent->d_left = swapNode;
      } else {
	swapNode->d_parent->d_right = swapNode;
      }
    }

    // swap pointers to parents
    temp = currentNode->d_parent;
    currentNode->d_parent = swapNode->d_parent;
    swapNode->d_parent = temp;

    if(d_root == currentNode) {
      d_root = swapNode;
    }

    if(currentNode == d_min) {
      d_min = swapNode;
    }

    if(currentNode->d_parent != NULL) {
      currentNode->d_parent->d_left = currentNode->d_right;
      currentNode->d_parent->d_leftMaxDepth = currentNode->d_rightMaxDepth;
      if(currentNode->d_parent->d_left != NULL) {
	currentNode->d_parent->d_left->d_parent = currentNode->d_parent;
      }
    }
    d_current = swapNode;
  } else {
    d_current = swapNode;
  }

  // Delete node
  Node *parent = currentNode->d_parent;
  if(parent != NULL) {
    // Balance tree after removing node
    long difference = parent->d_rightMaxDepth - parent->d_leftMaxDepth;
    if(difference > 1) {
      parent = parent->d_right;
    } else if(difference < -1) {
      parent = parent->d_left;
    } else if(parent->d_parent != NULL) {
      int depth = parent->d_leftMaxDepth;
      if(depth < parent->d_rightMaxDepth) {
	depth = parent->d_rightMaxDepth;
      }
      depth++;
      if(parent->d_parent->d_left == parent) {
	parent->d_parent->d_leftMaxDepth = depth;
      } else {
	parent->d_parent->d_rightMaxDepth = depth;
      }
    }
    Balance(parent);
  }

  data = currentNode->d_data;
  delete currentNode;
  d_size--;

  return data;
}

template <class T, class KEY>
bool AvlTree<T, KEY>::IsEmpty(void) {
  if(d_size == 0) {
    return true;
  }

  return false;
}

template <class T, class KEY>
int AvlTree<T, KEY>::Size(void) {
  return d_size;
}

template <class T, class KEY>
void AvlTree<T, KEY>::RemoveSubtree(Node *root) {
  Stack<Node *> *removal = new Stack<Node *>();

  if(root->d_parent != NULL) {
    d_current = root->d_parent;
    if(root->d_parent->d_left == root) {
      root->d_parent->d_leftMaxDepth = 0;
      root->d_parent->d_left = NULL;
    } else {
      root->d_parent->d_rightMaxDepth = 0;
      root->d_parent->d_right = NULL;
    }
    // Need to update d_min and d_max
    // Also need balance tree
  } else {
    d_current = d_min = d_max = d_root = NULL;
  }
  Node *currentNode = root;

  while(removal->IsEmpty()) {
    if(currentNode == NULL) {
      currentNode = removal->Pop();
    } else if(currentNode->d_left != NULL || currentNode->d_right != NULL) {
      removal->Push(currentNode);
      removal->Push(currentNode->d_left);
      removal->Push(currentNode->d_right);
      currentNode = currentNode->d_right;
    } else {
      delete currentNode;
      currentNode = removal->Pop();
    }
  }

  delete removal;

  return;
}

template <class T, class KEY>
void AvlTree<T, KEY>::RotateLeft(Node *node) {
  Node *temp = node->d_right;
  temp->d_parent = node->d_parent;
  if(node->d_parent != NULL) {
    if(node->d_parent->d_left == node) {
      node->d_parent->d_left = temp;
    } else {
      node->d_parent->d_right = temp;
    }
  }

  if(temp->d_left != NULL) {
    temp->d_left->d_parent = node;
  }
  node->d_right = temp->d_left;
  node->d_rightMaxDepth = temp->d_leftMaxDepth;
  node->d_parent = temp;

  temp->d_left = node;
  int depth = node->d_leftMaxDepth;
  if(depth < node->d_rightMaxDepth) {
    depth = node->d_rightMaxDepth;
  }
  temp->d_leftMaxDepth = depth + 1;

  return;
}

template <class T, class KEY>
void AvlTree<T, KEY>::RotateRight(Node *node) {
  Node *temp = node->d_left;
  temp->d_parent = node->d_parent;
  if(node->d_parent != NULL) {
    if(node->d_parent->d_left == node) {
      node->d_parent->d_left = temp;
    } else {
      node->d_parent->d_right = temp;
    }
  }

  if(temp->d_right != NULL) {
    temp->d_right->d_parent = node;
  }
  node->d_left = temp->d_right;
  node->d_leftMaxDepth = temp->d_rightMaxDepth;
  node->d_parent = temp;

  temp->d_right = node;
  int depth = node->d_leftMaxDepth;
  if(depth < node->d_rightMaxDepth) {
    depth = node->d_rightMaxDepth;
  }
  temp->d_rightMaxDepth = depth + 1;

  return;
}

template <class T, class KEY>
void AvlTree<T, KEY>::LargestDataKey(T *data, KEY *key) {
  *data = (d_max->d_data);
  *key =  (d_max->d_key);

  return;
}

template <class T, class KEY>
void AvlTree<T, KEY>::SmallestDataKey(T *data, KEY *key) {
  *data = (d_min->d_data);
  *key =  (d_min->d_key);

  return;
}

template <class T, class KEY>
bool AvlTree<T,KEY>::SetLocation(KEY key) {
  Node *temp = d_root;

  while(temp != NULL) {
    if(temp->d_key == key) {
      d_current = temp;
      return true;
    }
    if(key > temp->d_key) {
      temp = temp->d_right;
    } else {
      temp = temp->d_left;
    }
  }

  return  false;
}

template <class T, class KEY>
bool AvlTree<T,KEY>::SetMax(void) {
  if(d_max == NULL) {
    return false;
  }
  d_current = d_max;
  return true;
}

template <class T, class KEY>
bool AvlTree<T,KEY>::SetMin(void) {
  if(d_min == NULL) {
    return false;
  }
  d_current = d_min;
  return true;
}

template <class T, class KEY>
T AvlTree<T,KEY>::Next(void) {
  if(d_current == NULL || d_current == d_max) {
    return (T)0;
  }

  if(d_current->d_right == NULL) {
    while(d_current->d_parent != NULL && 
	  d_current->d_parent->d_right == d_current) {
      d_current = d_current->d_parent;
    }
    if(d_current->d_parent == NULL) {
      d_current = d_max;
      return (T)0;
    }
    d_current = d_current->d_parent;
  } else {
    d_current = d_current->d_right;
    while(d_current->d_left != NULL) {
      d_current = d_current->d_left;
    }
  }

  return d_current->d_data;
}

template <class T, class KEY>
T AvlTree<T,KEY>::NextLoop(void) {
  if(d_current == NULL) {
    return (T)0;
  }
  if(d_current == d_max) {
    d_current = d_min;
    return d_current->d_data;
  }

  if(d_current->d_right == NULL) {
    while(d_current->d_parent != NULL && 
	  d_current->d_parent->d_right == d_current) {
      d_current = d_current->d_parent;
    }
    if(d_current->d_parent == NULL) {
      d_current = d_max;
      return (T)0;
    }
    d_current = d_current->d_parent;
  } else {
    d_current = d_current->d_right;
    while(d_current->d_left != NULL) {
      d_current = d_current->d_left;
    }
  }

  return d_current->d_data;
}

template <class T, class KEY>
T AvlTree<T,KEY>::Previous(void) {
  if(d_current == NULL || d_current == d_min) {
    return (T)0;
  }

  if(d_current->d_left == NULL) {
    while(d_current->d_parent != NULL && 
	  d_current->d_parent->d_left == d_current) {
      d_current = d_current->d_parent;
    }
    if(d_current->d_parent == NULL) {
      d_current = d_min;
      return (T)0;
    }
    d_current = d_current->d_parent;
  } else {
    d_current = d_current->d_left;
    while(d_current->d_right != NULL) {
      d_current = d_current->d_right;
    }
  }

  return d_current->d_data;
}

template <class T, class KEY>
T AvlTree<T,KEY>::PreviousLoop(void) {
  if(d_current == NULL) {
    return (T)0;
  }
  if(d_current == d_min) {
    d_current = d_max;
    return d_current->d_data;
  }

  if(d_current->d_left == NULL) {
    while(d_current->d_parent != NULL && 
	  d_current->d_parent->d_left == d_current) {
      d_current = d_current->d_parent;
    }
    if(d_current->d_parent == NULL) {
      d_current = d_min;
      return (T)0;
    }
    d_current = d_current->d_parent;
  } else {
    d_current = d_current->d_left;
    while(d_current->d_right != NULL) {
      d_current = d_current->d_right;
    }
  }

  return d_current->d_data;
}

template <class T, class KEY>
T AvlTree<T,KEY>::GetCurrentData(void) {
  if(d_current != NULL) {
    return d_current->d_data;
  } else {
    return (T)0;
  }
}

template <class T, class KEY>
KEY AvlTree<T,KEY>::GetCurrentKey(void) {
  if(d_current != NULL) {
    return d_current->d_key;
  } else {
    return (KEY)0;
  }
}

template <class T, class KEY>
void AvlTree<T,KEY>::Print(void) {
  PrintTree(d_root);
  cout << "Print done\n";
}

template <class T, class KEY>
void AvlTree<T,KEY>::PrintTree(Node *temp) {
  cout << "Node: key " << temp->d_key << " value " << temp->d_data << endl;
  cout << "Children: left: ";
  if(temp->d_left == NULL) {
    cout << "FALSE ";
  } else {
    cout << "TRUE ";
  }
  cout << "right: ";
  if(temp->d_right == NULL) {
    cout << "FALSE ";
  } else {
    cout << "TRUE ";
  }
  cout << endl;
  cout << "Parent: ";
  if(temp->d_parent != NULL) {
    cout << temp->d_parent->d_key;
  } else {
    cout << "NULL";
  }
  cout << endl << endl;
  if(temp->d_left != NULL) {
    cout << "Going left" << endl;
    PrintTree(temp->d_left);
  }
  if(temp->d_right != NULL) {
    cout << "Going right" << endl;
    PrintTree(temp->d_right);
  }

  return;
}

template <class T, class KEY> 
void AvlTree<T,KEY>::Balance(Node *currentNode) {
  if(currentNode == NULL) {
    return;
  }

  Node *parent = currentNode->d_parent;
  while(parent != NULL) {
    long difference = parent->d_leftMaxDepth - parent->d_rightMaxDepth;
    if(difference > 1) {
      RotateRight(parent);
    } else if(difference < -1) {
      RotateLeft(parent);
    } else {
      currentNode = parent;
    }
    if(parent == d_root) {
      Node *temp = currentNode;
      while(temp->d_parent != NULL) {
	temp = temp->d_parent;
      }
      d_root = temp;
    }

    int depth = currentNode->d_rightMaxDepth;
    if(depth < currentNode->d_leftMaxDepth) {
      depth = currentNode->d_leftMaxDepth;
    }
    depth++;
    if(currentNode->d_parent != NULL) {
      if(currentNode->d_parent->d_left == currentNode) {
	currentNode->d_parent->d_leftMaxDepth = depth;
      } else {
	currentNode->d_parent->d_rightMaxDepth = depth;
      }
    }

    parent = currentNode->d_parent;
  }

  return;
}

#endif
