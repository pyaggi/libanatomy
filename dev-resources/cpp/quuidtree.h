#ifndef QUUIDTREE_H
#define QUUIDTREE_H
#include <cstddef>
#include <memory>
#include <map>
template <typename T,typename IDT>
class QuUidTreeNode
{
public:
    typedef std::map<IDT,QuUidTreeNode<T,IDT>*> IdMap;

    QuUidTreeNode(IdMap *m):m_idmap(m){}
    QuUidTreeNode(const T &t,IdMap *m=nullptr):m_data(t),m_idmap(m){}
    QuUidTreeNode(T &&t,IdMap *m=nullptr):m_data(std::move(t)),m_idmap(m){}

    const IDT & id() const {return m_data.id();}

    ~QuUidTreeNode()
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

    QuUidTreeNode<T,IDT> *nextSibling() {return m_next;}
    const QuUidTreeNode<T,IDT> *nextSibling() const {return m_next;}

    QuUidTreeNode<T,IDT> *parent() {return m_parent;}
    const QuUidTreeNode<T,IDT> *parent() const {return m_parent;}

    QuUidTreeNode<T,IDT> *prevSibling() {return m_prev;}
    const QuUidTreeNode<T,IDT> *prevSibling() const {return m_prev;}

    QuUidTreeNode<T,IDT> *firstChild() {return m_first;}
    const QuUidTreeNode<T,IDT> *firstChild() const {return m_first;}

    QuUidTreeNode<T,IDT> *lastChild()
    {
        if (m_first)
            return m_first->lastSibling();
        return nullptr;
    }
    const QuUidTreeNode<T,IDT> *lastChild() const
    {
        if (m_first)
            return m_first->lastSibling();
        return nullptr;
    }

    QuUidTreeNode<T,IDT> *lastSibling()
    {
        auto ret=this;
        while (ret && ret->m_next)
            ret=ret->m_next;
        return ret;
    }
    const QuUidTreeNode<T,IDT> *lastSibling() const
    {
        auto ret=this;
        while (ret->m_next)
            ret=ret->m_next;
        return ret;
    }

    QuUidTreeNode<T,IDT> *firstSibling()
    {
        auto ret=this;
        while (ret->m_prev)
            ret=ret->m_prev;
        return ret;
    }

    const QuUidTreeNode<T,IDT> *firstSibling() const
    {
        auto ret=this;
        while (ret && ret->m_prev)
            ret=ret->m_prev;
        return ret;
    }

    QuUidTreeNode<T,IDT> *root()
    {
        auto ret=this;
        while (ret->m_parent)
            ret=ret->m_parent;
        return ret;
    }
    const QuUidTreeNode<T,IDT> *root() const
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
        if (m_idmap)
        {
            auto f=m_idmap->find(id());
            if (f!=m_idmap->end())
                m_idmap->erase(f);
            m_idmap=nullptr;
        }
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
//    void swap(QuUidTreeNode<T,IDT> *b)
//    {
//        if (m_idmap)
//        {
//            auto ait=m_idmap->find(id());
//            auto bit=b->m_idmap->find(b->id());
//            m_idmap->swap(ait,bit);
//        }

//        if (m_parent && m_parent==b->m_parent)
//        {
//            if (m_parent->m_first==this)
//                m_parent->m_first=b;
//            else if (m_parent->m_first==b)
//                m_parent->m_first=this;
//        }
//        else
//        {
//            if (m_parent && m_parent->m_first==this)
//                m_parent->m_first=b;
//            if (b->m_parent && b->m_parent->m_first==b)
//                b->m_parent->m_first=this;
//            std::swap(m_parent,b->m_parent);
//        }
//        if (m_next==b)
//        {
//            b->m_prev=m_prev;
//            m_next=b->m_next;
//            b->m_next=this;
//            m_prev=b;
//        }
//        else if (m_prev==b)
//        {
//            m_prev=b->m_prev;
//            b->m_next=m_next;
//            m_next=b;
//            b->m_prev=this;
//        }
//        else
//        {
//            std::swap(m_prev,b->m_prev);
//            std::swap(m_next,b->m_next);
//        }

//        if (m_next==this)
//            m_next=nullptr;
//        else if (m_next)
//                m_next->m_prev=this;
//        if (m_prev==this)
//            m_prev=nullptr;
//        else if (m_prev)
//            m_prev->m_next=this;

//        if (b->m_next==b)
//            b->m_next=nullptr;
//        else if (b->m_next)
//            b->m_next->m_prev=b;

//        if (b->m_prev==b)
//            b->m_prev=nullptr;
//        else if (b->m_prev)
//            b->m_prev->m_next=b;


//    }
    void appendChild(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        if (!m_first)
        {
            node->m_idmap=m_idmap;
            (*m_idmap)[node->id()]=node;
            m_first=node;
            node->m_parent=this;
        }
        else
            m_first->appendLastSibling(node);
    }
    void prependChild(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        if (!m_first)
        {
            node->m_idmap=m_idmap;
            (*m_idmap)[node->id()]=node;
            m_first=node;
            node->m_parent=this;
        }
        else
            m_first->prependLastSibling(node);
    }
    void appendLastSibling(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        node->m_idmap=m_idmap;
        (*m_idmap)[node->id()]=node;
        auto ls=lastSibling();
        ls->m_next=node;
        node->m_prev=ls;
        node->m_parent=m_parent;
    }
    void prependLastSibling(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        node->m_idmap=m_idmap;
        (*m_idmap)[node->id()]=node;
        auto ls=firstSibling();
        ls->m_prev=node;
        node->m_next=ls;
        if (m_parent)
        {
            node->m_parent=m_parent;
            m_parent->m_first=node;
        }
    }
    void appendSibling(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        node->m_idmap=m_idmap;
        (*m_idmap)[node->id()]=node;
        if (m_next)
        {
            m_next->m_prev=node;
            node->m_next=m_next;
        }
        m_next=node;
        node->m_prev=this;
        node->m_parent=m_parent;
    }
    void prependSibling(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        node->m_idmap=m_idmap;
        (*m_idmap)[node->id()]=node;

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
    void insertPrev(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        node->m_idmap=m_idmap;
        (*m_idmap)[node->id()]=node;
        if (m_prev)
            m_prev->m_next=node;
        node->m_prev=m_prev;
        node->m_next=this;
        m_prev=node;
        node->m_parent=m_parent;
    }
    void insertNext(QuUidTreeNode<T,IDT> *node)
    {
        node->remove();
        node->m_idmap=m_idmap;
        (*m_idmap)[node->id()]=node;
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
    IdMap *m_idmap;
    T m_data;
    QuUidTreeNode<T,IDT> *m_parent=nullptr,*m_next=nullptr,*m_prev=nullptr,*m_first=nullptr;
};
template <typename T,typename IDT>
class QuUidTree:public QuUidTreeNode<T,IDT>
{
public:
    QuUidTree():QuUidTreeNode<T,IDT>(&m_idmap){}
    ~QuUidTree()
    {
        m_idmap.clear();
        clear();
    }
    typedef QuUidTreeNode<T,IDT> Node;
    void clear()
    {
        m_idmap.clear();
        auto fc=QuUidTreeNode<T,IDT>::firstChild();
        while (fc)
        {
            fc->remove();
            delete fc;
            fc=QuUidTreeNode<T,IDT>::firstChild();
        }
    }
    QuUidTreeNode<T,IDT> *node(const IDT &t) const
    {
        auto f=m_idmap.find(t);
        if (f==m_idmap.end())
            return nullptr;
        return f->second;
    }
    T &data(const IDT &t) {return m_idmap[t]->data();}
    const T &data(const IDT &t) const
    {
        auto f=m_idmap.find(t);
        if (f==m_idmap.end())
        {
            static T t;
            return t;
        }
        return f->second->data();
    }
    std::size_t idIndex(const IDT &t) const
    {
        auto f=m_idmap.find(t);
        if (f==m_idmap.end())
            return -1;
        return std::distance(m_idmap.begin(),f);
    }
    typename QuUidTreeNode<T,IDT>::IdMap *idMap() {return &m_idmap;}
    typename QuUidTreeNode<T,IDT>::IdMap const *idMap() const {return &m_idmap;}
private:
    typename QuUidTreeNode<T,IDT>::IdMap m_idmap;
    using QuUidTreeNode<T,IDT>::remove;
    using QuUidTreeNode<T,IDT>::appendLastSibling;
    using QuUidTreeNode<T,IDT>::prependLastSibling;
    using QuUidTreeNode<T,IDT>::insertNext;
    using QuUidTreeNode<T,IDT>::insertPrev;
};

template <typename T,typename IDT>
class QuUidTreeCursor
{
public:
    typedef QuUidTreeNode<T,IDT> Node;
    typedef QuUidTree<T,IDT> Tree;


    QuUidTreeCursor() {}
    QuUidTreeCursor(Tree *tree):m_tree(tree),m_cur(tree){}
    QuUidTreeCursor(Tree *tree,Node *node):m_tree(tree),m_cur(node){}

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

    QuUidTreeCursor endCursor() const
    {
        return QuUidTreeCursor(m_tree,nullptr);
    }
    QuUidTreeCursor rootCursor() const
    {
        return QuUidTreeCursor(m_tree,m_tree);
    }
    QuUidTreeCursor firstChildCursor() const
    {
        if (m_cur)
            return QuUidTreeCursor(m_tree,m_cur->firstChild());
        return QuUidTreeCursor(m_tree,m_tree->firstChild());
    }
    QuUidTreeCursor lastChildCursor() const
    {
        if (m_cur)
            return QuUidTreeCursor(m_tree,m_cur->lastChild());
        return QuUidTreeCursor(m_tree,m_tree->lastChild());
    }
    QuUidTreeCursor nextSiblingCursor() const
    {
        if (m_cur)
            return QuUidTreeCursor(m_tree,m_cur->nextSibling());
        return *this;
    }
    QuUidTreeCursor prevSiblingCursor() const
    {
        if (m_cur)
            return QuUidTreeCursor(m_tree,m_cur->prevSibling());
        return *this;
    }

    QuUidTreeCursor parentCursor() const
    {
        if (m_cur)
            return QuUidTreeCursor(m_tree,m_cur->parent());
        return QuUidTreeCursor(m_tree,nullptr);
    }

    QuUidTreeNode<T,IDT>* firstChildNode() const
    {
        if (m_cur)
            return m_cur->firstChild();
        return m_tree->firstChild();
    }
    QuUidTreeNode<T,IDT>* lastChildNode() const
    {
        if (m_cur)
            return m_cur->lastChild();
        return m_tree->lastChild();
    }
    QuUidTreeNode<T,IDT>* nextSiblingNode() const
    {
        if (m_cur)
            return m_cur->nextSibling();
        return nullptr;
    }
    QuUidTreeNode<T,IDT>* prevSiblingNode() const
    {
        if (m_cur)
            return m_cur->prevSibling();
        return nullptr;
    }
    QuUidTreeNode<T,IDT>* parentNode() const
    {
        if (m_cur)
            return m_cur->parent();
        return nullptr;
    }


    QuUidTreeCursor append(const T &data,bool makeCurrent)
    {
        return append(new Node(data),makeCurrent);
    }
    QuUidTreeCursor prepend(const T &data,bool makeCurrent)
    {
        return prepend(new Node(data),makeCurrent);
    }
    QuUidTreeCursor append(T &&data,bool makeCurrent)
    {
        return append(new Node(std::move(data)),makeCurrent);
    }
    QuUidTreeCursor prepend(T &&data,bool makeCurrent)
    {
        return prepend(new Node(std::move(data)),makeCurrent);
    }

    QuUidTreeCursor append(Node *node,bool makeCurrent)
    {
        if (m_cur)
            m_cur->appendChild(node);
        else
            m_tree->appendChild(node);
        if (makeCurrent)
            m_cur=node;
        return QuUidTreeCursor(m_tree,node);
    }
    QuUidTreeCursor prepend(Node *node,bool makeCurrent)
    {
        if (m_cur)
            m_cur->prependChild(node);
        else
            m_tree->prependChild(node);
        if (makeCurrent)
            m_cur=node;
        return QuUidTreeCursor(m_tree,node);
    }
    void swap(QuUidTreeCursor<T,IDT> &b)
    {
        m_cur->swap(b.m_cur);
    }
    bool operator==(const QuUidTreeCursor<T,IDT> &b) const {return m_tree==b.m_tree && m_cur==b.m_cur; }
    bool operator!=(const QuUidTreeCursor<T,IDT> &b) const {return m_tree!=b.m_tree || m_cur!=b.m_cur; }

private:
    Tree *m_tree=nullptr;
    Node *m_cur=nullptr;
};

template <typename T,typename IDT>
class QuUidTreeConstCursor
{
public:
    typedef QuUidTreeNode<T,IDT> Node;
    typedef QuUidTree<T,IDT> Tree;


    QuUidTreeConstCursor() {}
    QuUidTreeConstCursor(const Tree *tree):m_tree(tree),m_cur(tree){}
    QuUidTreeConstCursor(const Tree *tree,const Node *node):m_tree(tree),m_cur(node){}
    QuUidTreeConstCursor(const QuUidTreeCursor<T,IDT> &c):m_tree(c.m_tree),m_cur(c.m_cur){}

    bool isNull() const {return m_tree==nullptr;}
    bool atEnd() const {return m_tree==nullptr || m_cur==nullptr;}
    bool atRoot() const {return m_cur==m_tree;}

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

    const Tree *tree() const {return m_tree;}

    const T *operator->() const {return &m_cur->data();}

    const Node *node() const {return m_cur;}

    const T &data() const {return m_cur->data();}

    bool operator==(const Node *node) const {return m_cur==node;}
    bool operator!=(const Node *node) const {return m_cur!=node;}

    std::size_t depth() const
    {
        if (!m_cur)
            return 0;
        return m_cur->depth();
    }

    QuUidTreeConstCursor endCursor() const
    {
        return QuUidTreeConstCursor(m_tree,nullptr);
    }
    QuUidTreeConstCursor rootCursor() const
    {
        return QuUidTreeConstCursor(m_tree,m_tree);
    }
    QuUidTreeConstCursor firstChildCursor() const
    {
        if (m_cur)
            return QuUidTreeConstCursor(m_tree,m_cur->firstChild());
        return QuUidTreeConstCursor(m_tree,m_tree->firstChild());
    }
    QuUidTreeConstCursor lastChildCursor() const
    {
        if (m_cur)
            return QuUidTreeConstCursor(m_tree,m_cur->lastChild());
        return QuUidTreeConstCursor(m_tree,m_tree->lastChild());
    }
    QuUidTreeConstCursor nextSiblingCursor() const
    {
        if (m_cur)
            return QuUidTreeConstCursor(m_tree,m_cur->nextSibling());
        return *this;
    }
    QuUidTreeConstCursor prevSiblingCursor() const
    {
        if (m_cur)
            return QuUidTreeConstCursor(m_tree,m_cur->prevSibling());
        return *this;
    }

    QuUidTreeConstCursor parentCursor() const
    {
        if (m_cur)
            return QuUidTreeConstCursor(m_tree,m_cur->parent());
        return QuUidTreeConstCursor(m_tree,nullptr);
    }

    QuUidTreeNode<T,IDT>* firstChildNode() const
    {
        if (m_cur)
            return m_cur->firstChild();
        return m_tree->firstChild();
    }
    QuUidTreeNode<T,IDT>* lastChildNode() const
    {
        if (m_cur)
            return m_cur->lastChild();
        return m_tree->lastChild();
    }
    QuUidTreeNode<T,IDT>* nextSiblingNode() const
    {
        if (m_cur)
            return m_cur->nextSibling();
        return nullptr;
    }
    QuUidTreeNode<T,IDT>* prevSiblingNode() const
    {
        if (m_cur)
            return m_cur->prevSibling();
        return nullptr;
    }
    QuUidTreeNode<T,IDT>* parentNode() const
    {
        if (m_cur)
            return m_cur->parent();
        return nullptr;
    }


    bool operator==(const QuUidTreeConstCursor<T,IDT> &b) const {return m_tree==b.m_tree && m_cur==b.m_cur; }
    bool operator!=(const QuUidTreeConstCursor<T,IDT> &b) const {return m_tree!=b.m_tree || m_cur!=b.m_cur; }

private:
    const Tree *m_tree=nullptr;
    const Node *m_cur=nullptr;
};





#endif // QUUIDTREE_H
