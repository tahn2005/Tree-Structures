export class Node<K, V> {
  key: K;
  value: V;
  parent: Node<K, V> | null;
  left: Node<K, V> | null;
  right: Node<K, V> | null;
  constructor(key: K, value: V, parent: Node<K, V> | null) {
    this.key = key;
    this.value = value;
    this.parent = parent;
    this.left = null;
    this.right = null;
  }

  clone(): Node<K, V> {
    return new Node(this.key, this.value, null);
  }
}
export class BinarySearchTree<K, V> {
  root: Node<K, V> | null;
  constructor() {
    this.root = null;
  }
  protected internalBstInsert(key: K, value: V) {
    const newNode = new Node(key, value, null);
    if (this.root === null) {
      this.root = newNode;
      return;
    }
    let currentNode = this.root;
    while (true) {
      if (key === currentNode.key) {
        currentNode.value = value;
        return;
      }
      if (key < currentNode.key) {
        if (currentNode.left === null) {
          currentNode.left = newNode;
          newNode.parent = currentNode;
          return;
        }
        currentNode = currentNode.left;
      } else {
        if (currentNode.right === null) {
          currentNode.right = newNode;
          newNode.parent = currentNode;
          return;
        }
        currentNode = currentNode.right;
      }
    }
  }
  async insert(key: K, value: V, onStep?: (step: any) => Promise<void>) {
    const newNode = new Node(key, value, null);
    if (this.root === null) {
      this.root = newNode;
      if (onStep) await onStep({ type: "insert", key, parentKey: null });
      return;
    }
    let currentNode = this.root;
    while (true) {
      if (onStep) await onStep({ type: "visit", key: currentNode.key });
      if (key === currentNode.key) {
        currentNode.value = value;
        if (onStep) await onStep({ type: "update", key: currentNode.key });
        return;
      }
      if (key < currentNode.key) {
        if (currentNode.left === null) {
          currentNode.left = newNode;
          newNode.parent = currentNode;
          if (onStep)
            await onStep({
              type: "insert",
              key,
              parentKey: currentNode.key,
              direction: "left",
            });
          return;
        }
        currentNode = currentNode.left;
      } else {
        if (currentNode.right === null) {
          currentNode.right = newNode;
          newNode.parent = currentNode;
          if (onStep)
            await onStep({
              type: "insert",
              key,
              parentKey: currentNode.key,
              direction: "right",
            });
          return;
        }
        currentNode = currentNode.right;
      }
    }
  }
  remove(key: K, _onStep?: (step: any) => Promise<void>) {
    let node = this.internalFind(key);
    if (node === null) {
      return;
    }

    if (node.left !== null && node.right !== null) {
      this.nodeSwap(node, this.predecessor(node));
    }

    if (node.left === null && node.right === null) {
      this.noChild(node);
    } else {
      this.oneChild(node);
    }
  }
  internalFind(key: K): Node<K, V> | null {
    let currentNode = this.root;
    while (currentNode !== null) {
      if (key === currentNode.key) {
        return currentNode;
      }
      if (key < currentNode.key) {
        currentNode = currentNode.left;
      } else {
        currentNode = currentNode.right;
      }
    }
    return null;
  }
  predecessor(node: Node<K, V>): Node<K, V> {
    if (node.left !== null) {
      let currentNode = node.left;
      while (currentNode.right !== null) {
        currentNode = currentNode.right;
      }
      return currentNode;
    }
    let parent = node.parent;
    let currentNode = node;
    while (parent !== null && currentNode === parent.left) {
      currentNode = parent;
      parent = parent.parent;
    }
    return parent as Node<K, V>;
  }
  nodeSwap(n1: Node<K, V>, n2: Node<K, V>) {
    if (n1 === n2 || n1 === null || n2 === null) {
      return;
    }
    const n1p = n1.parent;
    const n1r = n1.right;
    const n1lt = n1.left;
    const n1isLeft = n1p !== null && n1 === n1p.left;

    const n2p = n2.parent;
    const n2r = n2.right;
    const n2lt = n2.left;
    const n2isLeft = n2p !== null && n2 === n2p.left;

    let temp;
    temp = n1.parent;
    n1.parent = n2.parent;
    n2.parent = temp;

    temp = n1.left;
    n1.left = n2.left;
    n2.left = temp;

    temp = n1.right;
    n1.right = n2.right;
    n2.right = temp;

    if (n1r !== null && n1r === n2) {
      n2.right = n1;
      n1.parent = n2;
    } else if (n2r !== null && n2r === n1) {
      n1.right = n2;
      n2.parent = n1;
    } else if (n1lt !== null && n1lt === n2) {
      n2.left = n1;
      n1.parent = n2;
    } else if (n2lt !== null && n2lt === n1) {
      n1.left = n2;
      n2.parent = n1;
    }

    if (n1p !== null && n1p !== n2) {
      if (n1isLeft) n1p.left = n2;
      else n1p.right = n2;
    }
    if (n1r !== null && n1r !== n2) {
      n1r.parent = n2;
    }
    if (n1lt !== null && n1lt !== n2) {
      n1lt.parent = n2;
    }

    if (n2p !== null && n2p !== n1) {
      if (n2isLeft) n2p.left = n1;
      else n2p.right = n1;
    }
    if (n2r !== null && n2r !== n1) {
      n2r.parent = n1;
    }
    if (n2lt !== null && n2lt !== n1) {
      n2lt.parent = n1;
    }

    if (this.root === n1) {
      this.root = n2;
    } else if (this.root === n2) {
      this.root = n1;
    }
  }
  noChild(node: Node<K, V>) {
    if (node.parent === null) {
      this.root = null;
    } else if (node.parent.left === node) {
      node.parent.left = null;
    } else {
      node.parent.right = null;
    }
  }
  oneChild(node: Node<K, V>) {
    const child = node.left !== null ? node.left : node.right;
    
    if (child !== null) {
        child.parent = node.parent;
    }

    if (node.parent === null) {
      this.root = child;
    } else if (node.parent.left === node) {
      node.parent.left = child;
    } else {
      node.parent.right = child;
    }
  }
  getSmallestNode(): Node<K, V> | null {
    if (this.root === null) {
      return null;
    }
    let currentNode = this.root;
    while (currentNode.left !== null) {
      currentNode = currentNode.left;
    }
    return currentNode;
  }
  getLargestNode(): Node<K, V> | null {
    if (this.root === null) {
      return null;
    }
    let currentNode = this.root;
    while (currentNode.right !== null) {
      currentNode = currentNode.right;
    }
    return currentNode;
  }
  clear() {
    this.root = null;
  }
  async find(key: K, onStep?: (step: any) => Promise<void>): Promise<Node<K, V> | null> {
    let currentNode = this.root;
    while (currentNode !== null) {
      if (onStep) await onStep({ type: "visit", key: currentNode.key });
      if (key === currentNode.key) {
        if (onStep) await onStep({ type: "found", key: currentNode.key });
        return currentNode;
      }
      if (key < currentNode.key) {
        currentNode = currentNode.left;
      } else {
        currentNode = currentNode.right;
      }
    }
    if (onStep) await onStep({ type: "not-found", key });
    return null;
  }
  async findMin(onStep?: (step: any) => Promise<void>) {
    let currentNode = this.root;
    if (currentNode === null) return null;

    while (currentNode.left !== null) {
      if (onStep) await onStep({ type: "visit", key: currentNode.key });
      currentNode = currentNode.left;
    }
    if (onStep) await onStep({ type: "found", key: currentNode.key });
    return currentNode;
  }
  async findMax(onStep?: (step: any) => Promise<void>) {
    let currentNode = this.root;
    if (currentNode === null) return null;

    while (currentNode.right !== null) {
      if (onStep) await onStep({ type: "visit", key: currentNode.key });
      currentNode = currentNode.right;
    }
    if (onStep) await onStep({ type: "found", key: currentNode.key });
    return currentNode;
  }
  
  async clone(): Promise<BinarySearchTree<K, V>> {
    const newTree = new (this.constructor as any)();
    if (this.root === null) {
      return newTree;
    }

    const cloneRecursive = (node: Node<K, V>): Node<K, V> => {
      const newNode = node.clone();
      
      if (node.left) {
        newNode.left = cloneRecursive(node.left);
        newNode.left.parent = newNode;
      }
      if (node.right) {
        newNode.right = cloneRecursive(node.right);
        newNode.right.parent = newNode;
      }
      return newNode;
    }

    newTree.root = cloneRecursive(this.root);
    return newTree;
  }
} 