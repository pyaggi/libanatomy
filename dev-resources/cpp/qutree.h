#ifndef QUTREE_H
#define QUTREE_H
#include <cstddef>
#include <memory>

template <typename T>
class QuTreeNode
{
public:
    QuTreeNode(){}
    QuTreeNode(const T &t):m_data(t){}
    QuTreeNode(T &&t):m_data(std::move(t)){}

    ~QuTreeNode()
    {
        remove();
        auto f=m_first;
        while (f)
        {
            m_first=m_first->m_next;
            delete f;
            f=m_first;
        }
    }
    const T &data() const {return m_data;}
    T &data() {return m_data;}

    QuTreeNode<T> *nextSibling() {return m_next;}
    const QuTreeNode<T> *nextSibling() const {return m_next;}

    QuTreeNode<T> *parent() {return m_parent;}
    const QuTreeNode<T> *parent() const {return m_parent;}

    QuTreeNode<T> *prevSibling() {return m_prev;}
    const QuTreeNode<T> *prevSibling() const {return m_prev;}

    QuTreeNode<T> *firstChild() {return m_first;}
    const QuTreeNode<T> *firstChild() const {return m_first;}

    QuTreeNode<T> *lastChild()
    {
        if (m_first)
            return m_first->lastSibling();
        return nullptr;
    }
    const QuTreeNode<T> *lastChild() const
    {
        if (m_first)
            return m_first->lastSibling();
        return nullptr;
    }

    QuTreeNode<T> *lastSibling()
    {
        auto ret=this;
        while (ret->m_next)
            ret=ret->m_next;
        return ret;
    }
    const QuTreeNode<T> *lastSibling() const
    {
        auto ret=this;
        while (ret->m_next)
            ret=ret->m_next;
        return ret;
    }

    QuTreeNode<T> *firstSibling()
    {
        auto ret=this;
        while (ret->m_prev)
            ret=ret->m_prev;
        return ret;
    }

    const QuTreeNode<T> *firstSibling() const
    {
        auto ret=this;
        while (ret->m_prev)
            ret=ret->m_prev;
        return ret;
    }

    QuTreeNode<T> *root()
    {
        auto ret=this;
        while (ret->m_parent)
            ret=ret->m_parent;
        return ret;
    }
    const QuTreeNode<T> *root() const
    {
        auto ret=this;
        while (ret->m_parent)
            ret=ret->m_parent;
        return ret;
    }
    std::size_t depth() const
    {
        std::size_t retv=0;
        auto p=m_parent;
        while (p)
        {
            retv++;
            p=p->m_parent;
        }
        return retv;
    }


    void remove()
    {
        if (m_parent)
        {
            if (m_parent->m_first==this)
                m_parent->m_first=m_next;
        }
        if (m_next)
            m_next->m_prev=m_prev;
        if (m_prev)
            m_prev->m_next=m_next;
        m_parent=nullptr;
        m_next=nullptr;
        m_prev=nullptr;
    }
    void swap(QuTreeNode<T> *b)
    {
        if (m_parent && m_parent==b->m_parent)
        {
            if (m_parent->m_first==this)
                m_parent->m_first=b;
            else if (m_parent->m_first==b)
                m_parent->m_first=this;
        }
        else
        {
            if (m_parent && m_parent->m_first==this)
                m_parent->m_first=b;
            if (b->m_parent && b->m_parent->m_first==b)
                b->m_parent->m_first=this;
            std::swap(m_parent,b->m_parent);
        }
        if (m_next==b)
        {
            b->m_prev=m_prev;
            m_next=b->m_next;
            b->m_next=this;
            m_prev=b;
        }
        else if (m_prev==b)
        {
            m_prev=b->m_prev;
            b->m_next=m_next;
            m_next=b;
            b->m_prev=this;
        }
        else
        {
            std::swap(m_prev,b->m_prev);
            std::swap(m_next,b->m_next);
        }

        if (m_next==this)
            m_next=nullptr;
        else if (m_next)
            m_next->m_prev=this;
        if (m_prev==this)
            m_prev=nullptr;
        else if (m_prev)
            m_prev->m_next=this;

        if (b->m_next==b)
            b->m_next=nullptr;
        else if (b->m_next)
            b->m_next->m_prev=b;

        if (b->m_prev==b)
            b->m_prev=nullptr;
        else if (b->m_prev)
            b->m_prev->m_next=b;


    }
    void appendChild(QuTreeNode<T> *node)
    {
        node->remove();
        if (!m_first)
        {
            m_first=node;
            node->m_parent=this;
        }
        else
            m_first->appendLastSibling(node);
    }
    void prependChild(QuTreeNode<T> *node)
    {
        node->remove();
        if (!m_first)
        {
            m_first=node;
            node->m_parent=this;
        }
        else
            m_first->prependLastSibling(node);
    }
    void appendLastSibling(QuTreeNode<T> *node)
    {
        node->remove();
        auto ls=lastSibling();
        ls->m_next=node;
        node->m_prev=ls;
        node->m_parent=m_parent;
    }
    void prependLastSibling(QuTreeNode<T> *node)
    {
        node->remove();
        auto ls=firstSibling();
        ls->m_prev=node;
        node->m_next=ls;
        if (m_parent)
        {
            node->m_parent=m_parent;
            m_parent->m_first=node;
        }
    }
    void appendSibling(QuTreeNode<T> *node)
    {
        node->remove();
        if (m_next)
        {
            m_next->m_prev=node;
            node->m_next=m_next;
        }
        m_next=node;
        node->m_prev=this;
        node->m_parent=m_parent;
    }
    void prependSibling(QuTreeNode<T> *node)
    {
        node->remove();
        if (m_prev)
        {
            m_prev->m_next=node;
            node->m_prev=m_prev;
        }
        else
        {
            if (m_parent)
                m_parent->m_first=node;
        }
        node->m_next=this;
        m_prev=node;
        node->m_parent=m_parent;
    }

    std::size_t childCount() const
    {
        if (!m_first)
            return 0;
        return m_first->nextSiblingsCount()+1;
    }
    std::size_t nextSiblingsCount() const
    {
        std::size_t ret=0;
        auto n=m_next;
        while (n)
        {
            ret++;
            n=n->m_next;
        }
        return ret;
    }
    std::size_t prevSiblingsCount() const
    {
        std::size_t ret=0;
        auto n=m_prev;
        while (n)
        {
            ret++;
            n=n->m_prev;
        }
        return ret;
    }
    void insertPrev(QuTreeNode<T> *node)
    {
        node->remove();
        if (m_prev)
            m_prev->m_next=node;
        node->m_prev=m_prev;
        node->m_next=this;
        m_prev=node;
        node->m_parent=m_parent;
    }
    void insertNext(QuTreeNode<T> *node)
    {
        node->remove();
        if (m_next)
            m_next->m_prev=node;
        node->m_prev=this;
        node->m_next=m_next;
        m_next=node;
        node->m_parent=m_parent;
    }
    T &operator*() {return m_data;}
    const T &operator*() const {return m_data;}

    T *operator->() {return &m_data;}
    const T *operator->() const {return &m_data;}

private:
    T m_data;
    QuTreeNode<T> *m_parent=nullptr,*m_next=nullptr,*m_prev=nullptr,*m_first=nullptr;
};

template <typename T>
class QuTree:public QuTreeNode<T>
{
public:
    typedef QuTreeNode<T> Node;
    void clear()
    {
        auto fc=QuTreeNode<T>::firstChild();
        while (fc)
        {
            fc->remove();
            delete fc;
            fc=QuTreeNode<T>::firstChild();
        }
    }
private:
    using QuTreeNode<T>::remove;
    using QuTreeNode<T>::appendLastSibling;
    using QuTreeNode<T>::prependLastSibling;
    using QuTreeNode<T>::insertNext;
    using QuTreeNode<T>::insertPrev;
};

template <typename T>
class QuTreeCursor
{
public:
    typedef QuTreeNode<T> Node;
    typedef QuTree<T> Tree;


    QuTreeCursor() {}
    QuTreeCursor(Tree *tree):m_tree(tree),m_cur(tree){}
    QuTreeCursor(Tree *tree,Node *node):m_tree(tree),m_cur(node){}

    bool isNull() const {return m_tree==nullptr;}
    bool atEnd() const {return m_tree==nullptr || m_cur==nullptr;}
    bool atRoot() const {return m_cur==m_tree;}

    void remove()
    {
        if (m_cur!=m_tree)
            m_cur->remove();
    }
    bool moveUp()
    {
        if (m_cur && m_cur!=m_tree)
        {
            m_cur=m_cur->parent();
            return true;
        }
        return false;
    }
    bool moveNext()
    {
        if (m_cur)
            m_cur=m_cur->nextSibling();
        return m_cur!=nullptr;
    }
    bool movePrev()
    {
        if (m_cur)
            m_cur=m_cur->prevSibling();
        return m_cur!=nullptr;
    }
    bool moveIn()
    {
        if (m_cur)
            m_cur=m_cur->firstChild();
        return m_cur!=nullptr;
    }
    bool moveToLastChild()
    {
        if (!moveIn())
            return false;
        m_cur=m_cur->lastSibling();
        return true;
    }
    void moveToRoot()
    {
        m_cur=m_tree;
    }
    bool moveToFirstSibling()
    {
        if (m_cur)
            m_cur=m_cur->firstSibling();
        return m_cur!=nullptr;
    }
    bool moveToLastSibling()
    {
        if (m_cur)
            m_cur=m_cur->lastSibling();
        return m_cur!=nullptr;
    }

    Tree *tree() const {return m_tree;}

    T *operator->() {return &m_cur->data();}
    const T *operator->() const {return &m_cur->data();}

    Node *node() {return m_cur;}
    const Node *node() const {return m_cur;}

    T &data() {return m_cur->data();}
    const T &data() const {return m_cur->data();}

    bool operator==(const Node *node) const {return m_cur==node;}
    bool operator!=(const Node *node) const {return m_cur!=node;}

    std::size_t depth() const
    {
        if (!m_cur)
            return 0;
        return m_cur->depth();
    }

    QuTreeCursor endCursor() const
    {
        return QuTreeCursor(m_tree,nullptr);
    }
    QuTreeCursor rootCursor() const
    {
        return QuTreeCursor(m_tree,m_tree);
    }
    QuTreeCursor firstChildCursor() const
    {
        if (m_cur)
            return QuTreeCursor(m_tree,m_cur->firstChild());
        return QuTreeCursor(m_tree,m_tree->firstChild());
    }
    QuTreeCursor lastChildCursor() const
    {
        if (m_cur)
            return QuTreeCursor(m_tree,m_cur->lastChild());
        return QuTreeCursor(m_tree,m_tree->lastChild());
    }
    QuTreeCursor nextSiblingCursor() const
    {
        if (m_cur)
            return QuTreeCursor(m_tree,m_cur->nextSibling());
        return *this;
    }
    QuTreeCursor prevSiblingCursor() const
    {
        if (m_cur)
            return QuTreeCursor(m_tree,m_cur->prevSibling());
        return *this;
    }

    QuTreeCursor parentCursor() const
    {
        if (m_cur)
            return QuTreeCursor(m_tree,m_cur->parent());
        return QuTreeCursor(m_tree,nullptr);
    }

    QuTreeNode<T>* firstChildNode() const
    {
        if (m_cur)
            return m_cur->firstChild();
        return m_tree->firstChild();
    }
    QuTreeNode<T>* lastChildNode() const
    {
        if (m_cur)
            return m_cur->lastChild();
        return m_tree->lastChild();
    }
    QuTreeNode<T>* nextSiblingNode() const
    {
        if (m_cur)
            return m_cur->nextSibling();
        return nullptr;
    }
    QuTreeNode<T>* prevSiblingNode() const
    {
        if (m_cur)
            return m_cur->prevSibling();
        return nullptr;
    }
    QuTreeNode<T>* parentNode() const
    {
        if (m_cur)
            return m_cur->parent();
        return nullptr;
    }


    QuTreeCursor append(const T &data,bool makeCurrent)
    {
        return append(new Node(data),makeCurrent);
    }
    QuTreeCursor prepend(const T &data,bool makeCurrent)
    {
        return prepend(new Node(data),makeCurrent);
    }
    QuTreeCursor append(T &&data,bool makeCurrent)
    {
        return append(new Node(std::move(data)),makeCurrent);
    }
    QuTreeCursor prepend(T &&data,bool makeCurrent)
    {
        return prepend(new Node(std::move(data)),makeCurrent);
    }

    QuTreeCursor append(Node *node,bool makeCurrent)
    {
        if (m_cur)
            m_cur->appendChild(node);
        else
            m_tree->appendChild(node);
        if (makeCurrent)
            m_cur=node;
        return QuTreeCursor(m_tree,node);
    }
    QuTreeCursor prepend(Node *node,bool makeCurrent)
    {
        if (m_cur)
            m_cur->prependChild(node);
        else
            m_tree->prependChild(node);
        if (makeCurrent)
            m_cur=node;
        return QuTreeCursor(m_tree,node);
    }
    void swap(QuTreeCursor<T> &b)
    {
        m_cur->swap(b.m_cur);
    }
    bool operator==(const QuTreeCursor<T> &b) const {return m_tree==b.m_tree && m_cur==b.m_cur; }
    bool operator!=(const QuTreeCursor<T> &b) const {return m_tree!=b.m_tree || m_cur!=b.m_cur; }

private:
    Tree *m_tree=nullptr;
    Node *m_cur=nullptr;
};





#endif // QUTREE_H
