//
//  Binary Search Tree
//
//  Copyright (c) 2017 Olivier Cuisenaire. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <queue>
#include <cassert>
#include <stdexcept>

using namespace std;

template < typename T >
class BinarySearchTree {
public:

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;

private:
  /**
   *  @brief Noeud de l'arbre.
   *
   * contient une cle et les liens vers les sous-arbres droit et gauche.
   */
  struct Node {
    const value_type key; // clé non modifiable
    Node* right;          // sous arbre avec des cles plus grandes
    Node* left;           // sous arbre avec des cles plus petites
    size_t nbElements;    // nombre de noeuds dans le sous arbre dont
                          // ce noeud est la racine

    Node(const_reference key)  // seul constructeur disponible. key est obligatoire
    : key(key), right(nullptr), left(nullptr), nbElements(1)
    {
      cout << "(C" << key << ") ";
    }
    ~Node()               // destructeur
    {
      cout << "(D" << key << ") ";
    }
    Node() = delete;             // pas de construction par défaut
    Node(const Node&) = delete;  // pas de construction par copie
    Node(Node&&) = delete;       // pas de construction par déplacement
  };

  /**
   *  @brief  Racine de l'arbre. nullptr si l'arbre est vide
   */
  Node* _root;

public:
  /**
   *  @brief Constructeur par défaut. Construit un arbre vide
   */
  BinarySearchTree() : _root(nullptr){
  }

  /**
   *  @brief Constucteur de copie.
   *
   *  @param other le BinarySearchTree à copier
   *  Pour crée une copie de l'arbre, on utilise une queue qu'on remplira de tout les
   *  "root" des subtree à traiter.
   *  L'idée est de créer un nouveau Node, mais laisser les liens left et right du node
   *  pointer sur les enfants de l'arbre à copier
   *
   */
  BinarySearchTree( BinarySearchTree& other ):_root(nullptr) {
	if(other._root != nullptr){
		_root = new Node(other._root->key);
		copyInOrder(other._root, _root);
	}
  }
  /**
   *  @brief Opérateur d'affectation par copie.
   *
   *  @param other le BinarySearchTree à copier
   *
   */
  BinarySearchTree& operator= ( BinarySearchTree& other ) {
    this->BinarySearchTree::~BinarySearchTree();
 	new(this) BinarySearchTree<value_type>(other);
	return *this;
  }

  /**
   *  @brief Echange le contenu avec un autre BST
   *
   *  @param other le BST avec lequel on echange le contenu
   *
   */
  void swap(BinarySearchTree& other ) noexcept {
	  Node* tmp = _root;
	  _root = other._root;
	  other._root = tmp;
  }

  /**
   *  @brief constructeur de copie par déplacement
   *
   *  @param other le BST dont on vole le contenu
   *
   */
  BinarySearchTree( BinarySearchTree&& other ) noexcept {
	//Appel à l'opération d'affectation par déplacement
	*this = move(other);
  }

  /**
   *  @brief Opérateur d'affectation par déplacement.
   *
   *  @param other le BST dont on vole le contenu
   *
   */
  BinarySearchTree& operator= ( BinarySearchTree&& other ) noexcept {
	_root = other._root;
  	other._root = nullptr;
    return *this;
  }

  //
  // @brief Destructeur
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée deleteSubTree(Node*)
  //
  ~BinarySearchTree() {
    deleteSubTree( _root );
  }

private:
	/**
	 * TODO
	 * @param other   [description]
	 * @param newNode [description]
	 */
  void copyInOrder(Node* other,Node* copyNode){
	  if(other != nullptr){
		  //Copie du nb element
		  copyNode->nbElements = other->nbElements;
		  //Copie l'enfant de gauche
		  if(other->left != nullptr){
		  	Node* newLeftNode = new Node(other->left->key);
			copyNode->left = newLeftNode;
			copyInOrder(other->left,copyNode->left);
		  }
		  //Copie de l'enfant de droite
		  if(other->right != nullptr){
		  	Node* newRightNode = new Node(other->right->key);
			copyNode->right = newRightNode;
			copyInOrder(other->right,copyNode->right);
		  }
	  }
  }
  //
  // @brief Fonction détruisant (delete) un sous arbre
  //
  // @param r la racine du sous arbre à détruire.
  //          peut éventuellement valoir nullptr
  //
  static void deleteSubTree(Node* r) noexcept {
	if(r == nullptr) return;
    //check si il y a des enfants
	if(r->left != nullptr)
		deleteSubTree(r->left);
	if(r->right != nullptr)
		deleteSubTree(r->right);

	delete r;
  }

public:
  //
  // @brief Insertion d'une cle dans l'arbre
  //
  // @param key la clé à insérer.
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée insert(Node*&,const_reference)
  //
  void insert( const_reference key) {
    insert(_root,key);
  }

private:
  //
  // @brief Insertion d'une cle dans un sous-arbre
  //
  // @param r la racine du sous-arbre dans lequel
  //          insérer la cle.
  // @param key la clé à insérer.
  //
  // @return vrai si la cle est inseree. faux si elle etait deja presente.
  //
  // Si la cle est deja presente, cette fonction ne fait rien.
  // x peut éventuellement valoir nullptr en entrée.
  // la fonction peut modifier x, reçu par référence, si nécessaire
  //
  static bool insert(Node*& r, const_reference key) {

	//Si la clé est déja présente dans l'arbre
	//On doit être sur que la clé n'est pas déjà présente car
	//on incrémentera les compteurs sur chaques noeus
	if(contains(r,key)){
		return false;
	}
	//Si on arrive sur une feuille
    if(r == nullptr){
		r = new Node(key);
		return true;
	}
	r->nbElements++;
	if(key < r->key){
		insert(r->left,key);
	}else if(key > r->key){
		insert(r->right,key);
	}
  }

public:
  //
  // @brief Recherche d'une cle.
  //
  // @param key la cle a rechercher
  //
  // @return vrai si la cle trouvee, faux sinon.
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée contains(Node*,const_reference)
  //
  bool contains( const_reference key ) const noexcept {
    return contains(_root,key);
  }

private:
  //
  // @brief Recherche d'une cle dans un sous-arbre
  //
  // @param key la cle a rechercher
  // @param r   la racine du sous-arbre
  //
  // @return vrai si la cle trouvee, faux sinon.
  //
  static bool contains(Node* r, const_reference key) noexcept {
	if(r == nullptr){
		return false;
	}
    if(key < r->key){
		contains(r->left,key);
	}else if(key > r->key){
		contains(r->right,key);
	}else{
		return true;
	}
  }

public:
  //
  // @brief Recherche de la cle minimale.
  //
  // @return une const reference a la cle minimale
  //
  // @exception std::logic_error si necessaire
  //
  // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
  // Remarque MAURICE : Pour faire de manière recursive, il faudrait un paramètre dans cette fonction non ?
  const_reference min() const {
	if(_root == nullptr)
		throw logic_error("Tree is empty");
	Node * currentNode = _root;
    while(currentNode->left != nullptr){
		currentNode = currentNode->left;
	}
	return currentNode->key;
  }

  //
  // @brief Supprime le plus petit element de l'arbre.
  //
  // @exception std::logic_error si necessaire
  //
  // vous pouvez mettre en oeuvre de manière iterative ou recursive a choix
  //
  void deleteMin() {
  	if(_root == nullptr){
      throw logic_error("Tree is empty");    
    }
    	
    Node * currentNode = _root;
	  Node * parentNode = nullptr;
    while(currentNode->left != nullptr){
	   	parentNode = currentNode;
  		currentNode = currentNode->left;
      currentNode->nbElements--;
  	}
	//Si l'élément min à un enfant droite, on le raccroche au parent
 	//de l'élément min (a gauche)
	if(currentNode->right != nullptr){
		parentNode->left = currentNode->right;
	}else{
		parentNode->left = nullptr;
	}
    _root->nbElements--;
  	delete currentNode;
  }


  //
  // @brief Supprime l'element de cle key de l'arbre.
  //
  // @param key l'element a supprimer
  //
  // si l'element n'est pas present, la fonction ne modifie pas
  // l'arbre mais retourne false. Si l'element est present, elle
  // retourne vrai
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée deleteElement(Node*&,const_reference)
  //
  bool deleteElement( const_reference key) noexcept {
    return deleteElement( _root, key );
  }

private:
  //
  // @brief Supprime l'element de cle key du sous arbre.
  //
  // @param r la racine du sous arbre
  // @param key l'element a supprimer
  //
  // si l'element n'est pas present, la fonction ne modifie pas
  // l'arbre mais retourne false. Si l'element est present, elle
  // retourne vrai
  //
  static bool deleteElement( Node*& r, const_reference key) noexcept {
   //Si la clé n'est pas dans l'arbre
   if(!contains(r,key))
	   return false;

   if(key < r->key){
	   deleteElement(r->left,key);
	   r->nbElements--;
   }else if(key > r->key){
	   deleteElement(r->right,key);
	   r->nbElements--;
   }else{
	   //Element trouvé
	   Node* deleteNode = r;
	   if(r->right == nullptr){
		   r = r->left;
		   delete deleteNode;
		   return true;
	   }else if(r->left == nullptr){
		   r = r->right;
		   delete deleteNode;
		   return true;
	   //Suppression de Hibbard
	   }else{
		   //trouver l'élément min droite
		   Node* minRight = r->right;
		   Node* parentMinRight = r;

		   while(minRight->left != nullptr){
			   parentMinRight = minRight;
			   minRight = minRight->left;
         minRight->nbElements--;
		   }

       size_t x = r->nbElements-1;
		   //Enfant du parent de minRight = enfant droite minRight
		   parentMinRight->left = minRight->right;
		   //enfant du minRight = deleteNode->right
       r->right->nbElements--;
		   minRight->right = r->right;
		   minRight->left = r->left;
		   r = minRight;
       r->nbElements = x;
		   delete deleteNode;
		   return true;
	   }
   }
   return true;
 }
public:
  //
  // @brief taille de l'arbre
  //
  // @return le nombre d'elements de l'arbre
  //
  size_t size() const noexcept {
     return _root->nbElements;
  }

  //
  // @brief cle en position n
  //
  // @return une reference a la cle en position n par ordre croissant des
  // elements
  //
  // @exception std::logic_error si nécessaire
  //
  // ajoutez le code de gestion des exceptions, puis mettez en oeuvre
  // la fonction recursive nth_element(Node*, n)
  //
  const_reference nth_element(size_t n) const {
    if(n > _root->nbElements){
      throw logic_error("arbre has less elements than what you seek");
    }
    return nth_element(_root,n);
  }

private:
  //
  // @brief cle en position n dans un sous arbre
  //
  // @param r la racine du sous arbre. ne peut pas etre nullptr
  // @param n la position n
  //
  // @return une reference a la cle en position n par ordre croissant des
  // elements
  //
  static const_reference nth_element(Node* r, size_t n) noexcept {
    assert(r != nullptr);

    size_t nbElementLeftNode = 0;

    if(r->left) {
      nbElementLeftNode += r->left->nbElements;
    }

    if(n == nbElementLeftNode) {
      return r->key;
    }
    if(n < nbElementLeftNode) {
      return nth_element(r->left, n);
    } else if(n > nbElementLeftNode){
      return nth_element(r->right, n - nbElementLeftNode - 1);
    } 
  }

public:
  //
  // @brief position d'une cle dans l'ordre croissant des elements de l'arbre
  //
  // @param key la cle dont on cherche le rang
  //
  // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
  //
  // Ne pas modifier mais écrire la fonction
  // récursive privée rank(Node*,const_reference)
  //
  size_t rank(const_reference key) const noexcept {
    return rank(_root,key);
  }

private:
  //
  // @brief position d'une cle dans l'ordre croissant des elements du sous-arbre
  //
  // @param key la cle dont on cherche le rang
  // @param r la racine du sous arbre
  //
  // @return la position entre 0 et size()-1, size_t(-1) si la cle est absente
  //
   static size_t rank(Node* r, const_reference key) noexcept {

      if (r == nullptr) {
         return -1;
      }

      size_t nbElementLeftNode = 0;
      size_t nbElementRightNode = 0;
      // on vérifie si la partie de gauche est vide avant
      if (r->left != nullptr) {
         nbElementLeftNode = r->left->nbElements;
      }

      if (key < r->key) {
         return rank(r->left, key);
      } else if (key > r->key) {
         nbElementRightNode = rank(r->right, key);
  
         if (nbElementRightNode == -1) {
            return nbElementRightNode;
         }
         return nbElementRightNode + nbElementLeftNode + 1;
         
      } else{
         return nbElementLeftNode;
      }
   }
   
   
   
   
public:
  //
  // @brief linearise l'arbre
  //
  // transforme l'arbre en une liste simplement chainee composee des memes
  // noeuds que precedemment, mais dont les pointeurs left sont tous egaux
  // a nullptr. Cette liste doit toujours respecter les conditions d'un
  // arbre binaire de recherche
  //
  // Ne pas modifier cette fonction qui sert essentiellement a tester la
  // fonction recursive linearize(Node*, Node*&, size_t&) utilisée par
  // la methode publique arborize
  //
  void linearize() noexcept {
    size_t cnt = 0;
    Node* list = nullptr;
    linearize(_root,list,cnt);
    _root = list;
  }

private:
  //
  // @brief linearise une sous arbre tree en une liste
  //
  // @param tree pointe vers la racine de l'arbre a lineariser
  // @param list reference a la tete de la liste a creer. sera modifiee
  //             par la fonction pour que list pointe vers le plus petit
  //             element de tree
  // @param cnt  calcule au fure et a mesure le nombre d'elements de la liste
  //             cree. l'effet de la fonction doit etre d'ajouter le nombre
  //             d'elements du sous-arbre de racine tree. Cependant, vous
  //             avez uniquement le droit d'utiliser l'opérateur ++.
  //
  static void linearize(Node* tree, Node*& list, size_t& cnt) noexcept {
    /* ... */
  }

public:
  //
  // @brief equilibre l'arbre
  //
  // applique l'algorithme d'equilibrage de l'arbre par linearisation et
  // arborisation
  //
  // Ne pas modifier cette fonction.
  //
  void balance() noexcept {
    size_t cnt = 0;
    Node* list = nullptr;
    linearize(_root,list,cnt);
    arborize(_root,list,cnt);
  }

private:
  //
  // @brief arborise les cnt premiers elements d'une liste en un arbre
  //
  // @param tree reference dans laquelle il faut ecrire la racine de l'arbre
  //             arborise par la fonction
  // @param list IN - reference a la tete de la liste a parcourir. La liste
  //                  est composee de Node dont le pointer left est nullptr
  //             OUT - debut de la suite de la liste dont on a utilise cnt
  //                   elements
  // @param cnt  nombre d'elements de la liste que l'on doit utiliser pour
  //             arboriser le sous arbre
  //
  static void arborize(Node*& tree, Node*& list, size_t cnt) noexcept {
  }
  /*
  	TODO
   */
  template < typename Fn >
  void visitPreRecursif(Fn f, Node* node){
	if(node == nullptr)
		return;
	f(node->key);
	visitPreRecursif(f,node->left);
	visitPreRecursif(f,node->right);
  }
  /*
  	TODO
   */
  template < typename Fn >
  void visitSymRecursif(Fn f, Node* node){
   if(node == nullptr)
	   return;
   visitSymRecursif(f,node->left);
   f(node->key);
   visitSymRecursif(f,node->right);
  }
  /*
   TODO
   */
  template < typename Fn >
  void visitPostRecursif(Fn f, Node* node){
   if(node == nullptr)
	  return;
   visitPostRecursif(f,node->left);
   visitPostRecursif(f,node->right);
   f(node->key);
  }
public:
  //
  // @brief Parcours pre-ordonne de l'arbre
  //
  // @param f une fonction capable d'être appelée en recevant une cle
  //          en parametre. Pour le noeud n courrant, l'appel sera
  //          f(n->key);
  //
  template < typename Fn >
  void visitPre (Fn f) {
	  visitPreRecursif(f,_root);
  }

  //
  // @brief Parcours symétrique de l'arbre
  //
  // @param f une fonction capable d'être appelée en recevant une cle
  //          en parametre. Pour le noeud n courrant, l'appel sera
  //          f(n->key);
  //
  template < typename Fn >
  void visitSym (Fn f) {
	visitSymRecursif(f,_root);
  }

  //
  // @brief Parcours post-ordonne de l'arbre
  //
  // @param f une fonction capable d'être appelée en recevant une cle
  //          en parametre. Pour le noeud n courrant, l'appel sera
  //          f(n->key);
  //
  template < typename Fn >
  void visitPost (Fn f) {
	  visitPostRecursif(f,_root);
  }

  //
  // Les fonctions suivantes sont fournies pour permettre de tester votre classe
  // Merci de ne rien modifier au dela de cette ligne
  //
  void display() const {
    stringstream ss1, ss2, ss3;
    displayKeys(ss1);
    displayKeys(ss3);
    displayCounts(ss2);
    string l1, l2;

    size_t W = 11;
    while( getline(ss3,l1) )
      if (l1.length() > W) W = l1.length();

    displayKeys(ss1);

    cout << "\n";
    cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
    cout << "| "<< left << setw(W) << "key" << "| " << setw(W) << "nbElements" << "|" << endl;
    cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;
    while( getline(ss1,l1) and getline(ss2, l2) ) {
      cout << "| "<< left << setw(W) << l1 << "| " << setw(W) << l2 << "|" << endl;
    }
    cout << "+-" << left << setfill('-') << setw(W) << "-" << "+-" << setw(W) << "-" << setfill(' ') << "+" << endl;

  }

  void displayKeys(ostream& os = cout) const {
    display([](Node* n) -> const_reference { return n->key; }, os);
  }

  void displayCounts(ostream& os = cout) const {
    display([](Node* n) -> size_t { return n->nbElements; }, os);
  }

  //
  // utilise un parcours de l'arbre en largeur avec
  // un noeud sentinelle newLevel qui traque les
  // changements de niveaux
  //
  template <typename Fn>
  void display (Fn func, ostream& os = cout ) const {
    Node* newLevel = (Node*) -1;
    // addresse non nulle dont on est sur qu'elle ne contient pas
    // vraiment un Node. Utilisée comme sentinelle.

    queue<Node*> Q;
    Q.push(_root);
    Q.push(newLevel);

    while(!Q.empty()) {
      Node* cur = Q.front();
      Q.pop();

      if(cur == newLevel) {
        os << endl;
        if(!Q.empty())
          Q.push(newLevel);
      } else if(cur == nullptr) {
        os << "- ";
      } else {
        os << func(cur) << " ";
        Q.push(cur->left);
        Q.push(cur->right);
      }
    }
  }
};

//int main(){
//
//	BinarySearchTree<int> bst;
//	{
//	bst.insert(10);
//	bst.insert(12);
//	bst.insert(16);
//	bst.insert(15);
//	bst.insert(7);
//
//	bst.insert(5);
//	bst.insert(11);
//	bst.insert(4);
//	bst.insert(2);
//	bst.insert(6);
//
//	bst.insert(13);
//	bst.insert(14);
//	} //fill BST
//
//	 bst.deleteMin();
//	 bst.deleteMin();
//	bst.display();
//        cout << bst.rank(12);
//        
//	//bst.deleteElement(12);
//
//	//
//	//bst.visitPre([](int key){ cout << key << " ";});
//	// cout << "\n";
//	// bst.visitSym([](int key){ cout << key << " ";});
//	// cout << "\n";
//	// bst.visitPost([](int key){ cout << key << " ";});
//	// cout << "\n";
//
//	return EXIT_SUCCESS;
//}
