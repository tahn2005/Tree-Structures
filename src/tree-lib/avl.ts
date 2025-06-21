import { BinarySearchTree, Node } from "./bst";

export class AVLNode<K, V> extends Node<K, V> {
  height: number;
  constructor(key: K, value: V, parent: AVLNode<K, V> | null) {
    super(key, value, parent);
    this.height = 1;
  }

  clone(): AVLNode<K, V> {
    const newNode = new AVLNode(this.key, this.value, null);
    newNode.height = this.height;
    return newNode;
  }
}

export class AVLTree<K, V> extends BinarySearchTree<K, V> {
  async insert(key: K, value: V, onStep?: (step: any) => Promise<void>) {
    const newNode = new AVLNode(key, value, null);
    if (this.root === null) {
      this.root = newNode;
      if (onStep) await onStep({ type: 'insert', key: newNode.key, parentKey: null });
      return;
    }

    let currentNode = this.root as AVLNode<K, V>;
    while (true) {
      if (onStep) await onStep({ type: 'visit', key: currentNode.key });
      if (key === currentNode.key) {
        currentNode.value = value;
        if (onStep) await onStep({ type: 'update', key: currentNode.key });
        return;
      }

      if (key < currentNode.key) {
        if (currentNode.left === null) {
          currentNode.left = newNode;
          newNode.parent = currentNode;
          if (onStep) await onStep({ type: 'insert', key: newNode.key, parentKey: currentNode.key, direction: 'left' });
          if (currentNode.height === 1) {
            currentNode.height = 2;
            await this.insertFix(currentNode, newNode, onStep);
          }
          return;
        }
        currentNode = currentNode.left as AVLNode<K, V>;
      } else {
        if (currentNode.right === null) {
          currentNode.right = newNode;
          newNode.parent = currentNode;
          if (onStep) await onStep({ type: 'insert', key: newNode.key, parentKey: currentNode.key, direction: 'right' });
          if (currentNode.height === 1) {
            currentNode.height = 2;
            await this.insertFix(currentNode, newNode, onStep);
          }
          return;
        }
        currentNode = currentNode.right as AVLNode<K, V>;
      }
    }
  }

  async remove(key: K, onStep?: (step: any) => Promise<void>) {
    let node = this.internalFind(key) as AVLNode<K, V> | null;
    if (node === null) {
      if(onStep) await onStep({ type: "not-found", key });
      return;
    }
    if(onStep) await onStep({ type: "found", key });

    // Capture parent BEFORE any operations
    const parent = node.parent as AVLNode<K, V> | null;

    if (node.left !== null && node.right !== null) {
      const pred = this.predecessor(node) as AVLNode<K,V>;
      if(onStep) await onStep({ type: "swap", key1: node.key, key2: pred.key});
      this.nodeSwap(node, pred);
    }

    if(onStep) await onStep({type: 'remove', key: node.key});
    if (node.left === null && node.right === null) {
      this.noChild(node);
    } else {
      this.oneChild(node);
    }

    await this.removeFix(parent, onStep);
  }

  private async insertFix(p: AVLNode<K, V>, n: AVLNode<K, V>, onStep?: (step: any) => Promise<void>) {
    const g = p.parent as AVLNode<K, V> | null;
    if (g === null) {
      return;
    }
    
    const newHeight = Math.max(this.sub(g.left as AVLNode<K, V>), this.sub(g.right as AVLNode<K, V>)) + 1;
    if (g.height === newHeight) {
      return;
    }
    g.height = newHeight;
    if(onStep) await onStep({type: 'update-height', key: g.key, height: g.height});

    if (Math.abs(this.sub(g.left as AVLNode<K, V>) - this.sub(g.right as AVLNode<K, V>)) < 2) {
      await this.insertFix(g, p, onStep);
    } else {
      if (n === p.left) { 
        if (p === g.left) { 
          await this.rightRotate(g, onStep);
        } else { 
          await this.rightRotate(p, onStep);
          await this.leftRotate(g, onStep);
        }
      } else { 
        if (p === g.right) { 
          await this.leftRotate(g, onStep);
        } else { 
          await this.leftRotate(p, onStep);
          await this.rightRotate(g, onStep);
        }
      }
    }
  }

  private async removeFix(n: AVLNode<K, V> | null, onStep?: (step: any) => Promise<void>) {
    if (n === null) {
      return;
    }
    
    if (Math.abs(this.sub(n.left as AVLNode<K,V>) - this.sub(n.right as AVLNode<K,V>)) > 1) {
      const p = n.parent as AVLNode<K, V> | null;
      
      let c: AVLNode<K, V>; 
      if (this.sub(n.left as AVLNode<K,V>) > this.sub(n.right as AVLNode<K,V>)) {
        c = n.left as AVLNode<K, V>;
      } else {
        c = n.right as AVLNode<K, V>;
      }

      let g: AVLNode<K, V>;
      if (this.sub(c.left as AVLNode<K, V>) > this.sub(c.right as AVLNode<K, V>)) {
        g = c.left as AVLNode<K, V>;
      } else if (this.sub(c.left as AVLNode<K, V>) < this.sub(c.right as AVLNode<K, V>)) {
        g = c.right as AVLNode<K, V>;
      } else { 
        if (c === n.left) {
          g = c.left as AVLNode<K, V>;
        } else {
          g = c.right as AVLNode<K, V>;
        }
      }

      if (g === c.left) {
        if (c === n.left) { 
          await this.rightRotate(n, onStep);
        } else { 
          await this.rightRotate(c, onStep);
          await this.leftRotate(n, onStep);
        }
      } else {
        if (c === n.right) { 
          await this.leftRotate(n, onStep);
        } else { 
          await this.leftRotate(c, onStep);
          await this.rightRotate(n, onStep);
        }
      }
      await this.removeFix(p, onStep);
      return;
    }

    const oldHeight = n.height;
    n.height = Math.max(this.sub(n.left as AVLNode<K,V>), this.sub(n.right as AVLNode<K,V>)) + 1;
    if (n.height !== oldHeight) {
      if(onStep) await onStep({type: 'update-height', key: n.key, height: n.height});
      if (Math.abs(this.sub(n.left as AVLNode<K,V>) - this.sub(n.right as AVLNode<K,V>)) < 2) {
        await this.removeFix(n.parent as AVLNode<K,V> | null, onStep);
      }
    }
  }

  private async leftRotate(n: AVLNode<K, V>, onStep?: (step: any) => Promise<void>) {
    if(onStep) await onStep({ type: "rotate-left", key: n.key });
    const x = n.right as AVLNode<K, V>;
    const b = x.left as AVLNode<K, V> | null;
    const p = n.parent as AVLNode<K, V> | null;
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
    
    n.height = Math.max(this.sub(n.left as AVLNode<K, V>), this.sub(n.right as AVLNode<K, V>)) + 1;
    x.height = Math.max(this.sub(x.left as AVLNode<K, V>), this.sub(x.right as AVLNode<K, V>)) + 1;
    if (p !== null) {
      p.height = Math.max(this.sub(p.left as AVLNode<K, V>), this.sub(p.right as AVLNode<K, V>)) + 1;
    }
  }

  private async rightRotate(n: AVLNode<K, V>, onStep?: (step: any) => Promise<void>) {
    if(onStep) await onStep({ type: "rotate-right", key: n.key });
    const x = n.left as AVLNode<K, V>;
    const b = x.right as AVLNode<K, V> | null;
    const p = n.parent as AVLNode<K, V> | null;
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
    
    n.height = Math.max(this.sub(n.left as AVLNode<K, V>), this.sub(n.right as AVLNode<K, V>)) + 1;
    x.height = Math.max(this.sub(x.left as AVLNode<K, V>), this.sub(x.right as AVLNode<K, V>)) + 1;
    if (p !== null) {
      p.height = Math.max(this.sub(p.left as AVLNode<K, V>), this.sub(p.right as AVLNode<K, V>)) + 1;
    }
  }

  sub(n: AVLNode<K, V> | null): number {
    if (n === null) {
      return 0;
    }
    return n.height;
  }
  nodeSwap(n1: Node<K, V>, n2: Node<K, V>) {
    super.nodeSwap(n1, n2);
    const tempH = (n1 as AVLNode<K,V>).height;
    (n1 as AVLNode<K,V>).height = (n2 as AVLNode<K,V>).height;
    (n2 as AVLNode<K,V>).height = tempH;
  }
} 