import { BinarySearchTree, Node } from "./bst";

export class SplayTree<K, V> extends BinarySearchTree<K, V> {
  async insert(key: K, value: V, onStep?: (step: any) => Promise<void>) {
    const newNode = new Node(key, value, null);
    if (this.root === null) {
      this.root = newNode;
      await onStep?.({ type: "insert", key, value, isRoot: true });
      return;
    }

    let currentNode = this.root;
    while (true) {
      await onStep?.({ type: "compare", key: currentNode.key });
      if (key === currentNode.key) {
        currentNode.value = value;
        await onStep?.({ type: "update-value", key });
        await this.splay(currentNode, onStep);
        return;
      } else if (key < currentNode.key) {
        if (currentNode.left === null) {
          currentNode.left = newNode;
          newNode.parent = currentNode;
          await onStep?.({ type: "insert", key, value, parentKey: currentNode.key, direction: 'left' });
          await this.splay(newNode, onStep);
          return;
        }
        currentNode = currentNode.left;
      } else {
        if (currentNode.right === null) {
          currentNode.right = newNode;
          newNode.parent = currentNode;
          await onStep?.({ type: "insert", key, value, parentKey: currentNode.key, direction: 'right' });
          await this.splay(newNode, onStep);
          return;
        }
        currentNode = currentNode.right;
      }
    }
  }

  async remove(key: K, onStep?: (step: any) => Promise<void>) {
    const nodeToRemove = this.internalFind(key);
    if (nodeToRemove === null) {
      await onStep?.({ type: "not-found", key });
      return;
    }
    await onStep?.({ type: "found", key });

    const parentToSplay = nodeToRemove.parent;

    if (nodeToRemove.left !== null && nodeToRemove.right !== null) {
      const pred = this.predecessor(nodeToRemove);
       await onStep?.({ type: "swap", key1: nodeToRemove.key, key2: pred.key });
      this.nodeSwap(nodeToRemove, pred);
    }

    if (nodeToRemove.left === null && nodeToRemove.right === null) {
       await onStep?.({ type: "remove-no-child", key: nodeToRemove.key });
      this.noChild(nodeToRemove);
    } else {
       await onStep?.({ type: "remove-one-child", key: nodeToRemove.key });
      this.oneChild(nodeToRemove);
    }

    if (parentToSplay) {
      await this.splay(parentToSplay, onStep);
    }
  }

  async find(key: K, onStep?: (step: any) => Promise<void>) {
    if (this.root === null) {
      await onStep?.({type: 'not-found', key});
      return null;
    }

    let currentNode: Node<K, V> | null = this.root;
    let lastAccessed: Node<K, V> = this.root;

    while (currentNode !== null) {
      await onStep?.({ type: "visit", key: currentNode.key });
      lastAccessed = currentNode;
      
      if (key === currentNode.key) {
        await onStep?.({type: 'found', key});
        await this.splay(currentNode, onStep);
        return currentNode;
      } else if (key < currentNode.key) {
        currentNode = currentNode.left;
      } else {
        currentNode = currentNode.right;
      }
    }
    await onStep?.({type: 'not-found', key});
    await this.splay(lastAccessed, onStep);
    return null;
  }

  async findMin(onStep?: (step: any) => Promise<void>) {
    const node = this.getSmallestNode();
    if (node) {
      await this.splay(node, onStep);
    }
    return node;
  }

  async findMax(onStep?: (step: any) => Promise<void>) {
    const node = this.getLargestNode();
    if (node) {
      await this.splay(node, onStep);
    }
    return node;
  }

  async splay(n: Node<K, V> | null, onStep?: (step: any) => Promise<void>) {
    if (n === null) {
      return;
    }
    await onStep?.({ type: "splay", key: n.key });

    while (n.parent !== null) {
      const p: Node<K, V> = n.parent;
      const g = p.parent;
      if (g === null) {
        if (n === p.left) {
          await this.rightRotate(p, onStep);
        } else {
          await this.leftRotate(p, onStep);
        }
      } else {
        if (n === p.left) {
          if (p === g.left) {
            await this.rightRotate(g, onStep);
            await this.rightRotate(p, onStep);
          } else {
            await this.rightRotate(p, onStep);
            await this.leftRotate(g, onStep);
          }
        } else {
          if (p === g.right) {
            await this.leftRotate(g, onStep);
            await this.leftRotate(p, onStep);
          } else {
            await this.leftRotate(p, onStep);
            await this.rightRotate(g, onStep);
          }
        }
      }
    }
    this.root = n;
  }

  private async leftRotate(n: Node<K, V>, onStep?: (step: any) => Promise<void>) {
    await onStep?.({ type: "rotate-left", key: n.key });
    const x = n.right as Node<K, V>;
    const b = x.left as Node<K, V> | null;
    const p = n.parent as Node<K, V> | null;
    x.left = n;
    if (b !== null) {
      b.parent = n;
    }
    x.parent = p;
    n.right = b;
    n.parent = x;
    if (p === null) {
      this.root = x;
    } else if (n === p.left) {
      p.left = x;
    } else {
      p.right = x;
    }
  }

  private async rightRotate(n: Node<K, V>, onStep?: (step: any) => Promise<void>) {
    await onStep?.({ type: "rotate-right", key: n.key });
    const x = n.left as Node<K, V>;
    const b = x.right as Node<K, V> | null;
    const p = n.parent as Node<K, V> | null;
    x.right = n;
    if (b !== null) {
      b.parent = n;
    }
    x.parent = p;
    n.left = b;
    n.parent = x;
    if (p === null) {
      this.root = x;
    } else if (n === p.left) {
      p.left = x;
    } else {
      p.right = x;
    }
  }
} 

