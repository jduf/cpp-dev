#ifndef DEF_LIST
#define DEF_LIST

#include <memory>
#include "IOFiles.hpp"

/*{Description*/
/*! The list is constructed according this scheme
\verbatim
    ________________________
   |   *t_  *t_  *t_  *t_   |
   |    ↑    ↑    ↑    ↑    |
   | x─→o←──→o←──→o←──→o─→> |
   | │       ^            | |
   | NULL              NULL |
   |                        |
   | o = Node               |
   | x = Node head_         |
   | > = Node tail_         |
   | ^ = Node target_       |
   |________________________|
\endverbatim
 * The list contains 3 Nodes.
 *
 * + head_ that has is such that head_::prev_=NULL.
 * + tail_ that has is such that tail_::next_=NULL.
 * + target_ that can point to any Node
 *
 * Between head_ and tail_ there could be an many node as one wants. Each node
 * is only aware of the previous and next one.
 *
 * A node points to a given instance of Type.
 */
/*}*/
template<typename Type>
class List{
	public:
		List() = default;
		List(List&& l);
		~List();
		/*{Forbidden*/
		List(List<Type> const&) = delete;
		List& operator=(List) = delete;
		/*}*/

		class Node{
			public:
				Node(std::shared_ptr<Type> t, Node* prev, Node* next);
				~Node();
				/*{Forbidden*/
				Node(Node&&) = delete;
				Node() = delete;
				Node(Node const&) = delete;
				Node& operator=(List) = delete;
				/*}*/
				std::shared_ptr<Type> get() const { return t_; }

			protected:
				std::shared_ptr<Type> t_;
				Node* prev_;
				Node* next_;

				friend List<Type>;
		};

		void set();
		bool target_next() const;
		void set_target(List<Type>::Node* const target = NULL) const { target_ = target; }
		typename List<Type>::Node* get_target() const { return target_; }

		Type& operator[](unsigned int i){
			target_ = NULL;
			target_next();
			while(i--){ target_next(); }
			return (*target_->t_);
		}
		Type const& operator[](unsigned int i) const {
			target_ = NULL;
			target_next();
			while(i--){ target_next(); }
			return (*target_->t_);
		}

		std::shared_ptr<Type>& get_ptr(){ return target_->t_; }
		std::shared_ptr<Type> const& get_ptr() const { return target_->t_; }

		Type& get() { return (*target_->t_); }
		Type const& get() const { return (*target_->t_); }

		Type& first(){ return *head_->t_; }
		Type const& first() const { return *head_->t_; }

		Type& last() { return (*tail_->t_); }
		Type const& last() const { return (*tail_->t_); }

		void add_start(std::shared_ptr<Type> t);
		void add_end(std::shared_ptr<Type> t);
		void add(std::shared_ptr<Type> t, unsigned int const& idx);
		/*!Add t to the list at the correct place. If an entry t', such that
		 * t=t', is already contained in the list, it will add t anyway.*/
		void add_sort(std::shared_ptr<Type> t,  std::function<bool (Type const&, Type const&)> cmp);
		void add_after_target(std::shared_ptr<Type> t);

		void pop_start();
		void pop_end();
		void pop(unsigned int const& idx);
		void pop_range(unsigned int const& a, unsigned int const& b);

		/*!In a correctly sorted list, returns true if the list contains a t'
		 * such that cmp(t',t)=2 . In that case, targets_ point the that entry
		 * in the list. It returns false if the condition is not satisfied and
		 * targets_ points to the place where t should be added with
		 * add_after_target(t) to keep a correct sorting*/
		bool find_sorted(std::shared_ptr<Type> t, std::function<unsigned int (Type const&, Type const&)> cmp);
		/*!If find_sorted returns true, one can deal with the two identical (in the
		 * sense of cmp(t,t')=2) t and t'. The way to deal with the twin is is
		 * given by handling_function.*/
		void handle_twin(std::shared_ptr<Type> t, std::function<void (Type&, Type&)> handling_function);

		void swap(unsigned int const& a, unsigned int const& b);
		List<Type> sublist(unsigned int const& a, unsigned int const& b) const;

		unsigned int const& size() const { return size_; }

		std::string header_def() const { return "List("+my::tostring(size())+")"; }

	private:
		mutable Node* target_ = NULL;
		Node* head_           = NULL;
		Node* tail_           = NULL;
		unsigned int size_    = 0;
};

/*constructors and destructor*/
/*{*/
template<typename Type>
List<Type>::List(List&& l):
	target_(NULL),
	head_(l.head_),
	tail_(l.tail_),
	size_(l.size_)
{
	l.target_=NULL;
	l.head_ = NULL;
	l.tail_ = NULL;
	l.size_ = 0;
}

template<typename Type>
void List<Type>::set(){
	size_ = 0;
	delete head_;
	head_ = NULL;
	tail_ = NULL;
	target_ = NULL;
}

template<typename Type>
List<Type>::~List(){
	if(head_){ delete head_; }
}

template<typename Type>
List<Type>::Node::Node(std::shared_ptr<Type> t, Node* prev, Node* next):
	t_(t),
	prev_(prev),
	next_(next)
{}

template<typename Type>
List<Type>::Node::~Node(){
	if(next_){
		delete next_;
		next_ = NULL;
	}
}
/*}*/

/*i/o methods*/
/*{*/
template<typename Type>
std::ostream& operator<<(std::ostream& flux, List<Type> const& l){
	flux<<"("<<l.size()<<")";
	l.set_target();
	while(l.target_next()){ flux<<" "<<l.get(); }
	return flux;
}

template<typename Type>
std::istream& operator>>(std::istream& flux, List<Type>& l){
	unsigned int size;
	flux>>size;
	Type tmp;
	while(size--){
		flux>>tmp;
		l.add_end( std::make_shared<Type>(tmp) );
	}
	return flux;
}

template<typename Type>
IOFiles& operator<<(IOFiles& w, List<Type> const& l){
	if(w.is_binary()){
		w<<l.size();
		l.set_target();
		while(l.target_next()){ w<<l.get(); }
	} else { w.stream()<<l; }
	return w;
}

template<typename Type>
IOFiles& operator>>(IOFiles& r, List<Type>& l){
	if(r.is_binary()){
		unsigned int size;
		r>>size;
		while(size--){ l.add_end(std::make_shared<Type>(r.read<Type>())); }
	} else { r.stream()>>l; }
	return r;
}
/*}*/

/*add an element to the list*/
/*{*/
template<typename Type>
void List<Type>::add_start(std::shared_ptr<Type> t){
	if(head_){
		target_ = new Node(t,NULL,head_);
		head_->prev_ = target_;
		head_ = target_;
		target_ = NULL;
	} else {
		head_ = new Node(t,NULL,NULL);
		tail_ = head_;
	}
	size_++;
}

template<typename Type>
void List<Type>::add_end(std::shared_ptr<Type> t){
	if(head_){
		target_ = new Node(t,tail_,NULL);
		tail_->next_ = target_;
		tail_ = target_;
		target_ = NULL;
	} else {
		head_ = new Node(t,NULL,NULL);
		tail_ = head_;
	}
	size_++;
}

template<typename Type>
void List<Type>::add(std::shared_ptr<Type> t, unsigned int const& idx){
	if(idx == 0){ add_start(t); }
	else{
		target_ = head_;
		unsigned int i(0);
		while ( ++i<idx && target_ ) { target_ = target_->next_; }
		if(target_){
			if(target_->next_){
				target_->next_ = new Node(t,target_,target_->next_);
				target_->next_->next_->prev_ = target_->next_;
				target_ = NULL;
				size_++;
			} else { add_end(t); }
		} else  {
			std::cerr<<__PRETTY_FUNCTION__<<" : try to add a element outside the list range"<<std::endl;
		}
	}
}

template<typename Type>
void List<Type>::add_after_target(std::shared_ptr<Type> t){
	if(target_){
		if(target_->next_){
			target_->next_ = new Node(t,target_,target_->next_);
			target_->next_->next_->prev_ = target_->next_;
			target_ = NULL;
			size_++;
		} else { add_end(t); }
	} else { add_start(t); }
}

template<typename Type>
void List<Type>::add_sort(std::shared_ptr<Type> t, std::function<bool (Type const&, Type const&)> cmp){
	if(head_){
		if(head_->next_){
			if(cmp(*tail_->t_,*t)){ add_end(t); }
			else {
				if(cmp(*head_->t_,*t)){
					target_ = head_;
					while( cmp(*target_->next_->t_,*t) ){ target_ = target_->next_; }
					add_after_target(t);
				} else { add_start(t); }
			}
		} else {
			if( cmp(*head_->t_,*t) ){ add_end(t); }
			else { add_start(t); }
		}
	} else { add_end(t); }
}
/*}*/

/*remove an element from the list*/
/*{*/
template<typename Type>
void List<Type>::pop_start(){
	if(head_){
		if(head_->next_){
			target_ = head_;
			head_ = head_->next_;
			target_->next_ = NULL;
			delete target_;
			target_ = NULL;
		} else {
			delete head_;
			head_ = NULL;
			tail_ = NULL;
		}
		size_--;
	}
}

template<typename Type>
void List<Type>::pop_end(){
	if(head_){
		if(head_->next_){
			target_ = tail_;
			tail_ = tail_->prev_;
			tail_->next_ = NULL;
			delete target_;
			target_ = NULL;
		} else {
			delete head_;
			head_ = NULL;
			tail_ = NULL;
		}
		size_--;
	}
}

template<typename Type>
void List<Type>::pop(unsigned int const& idx){
	if(idx == 0){ pop_start(); }
	else {
		target_ = head_;
		unsigned int i(0);
		while ( i++<idx && target_ ){ target_ = target_->next_; }
		if(target_){
			if(target_->next_){
				target_->prev_->next_ = target_->next_;
				target_->next_->prev_ = target_->prev_;
				target_->next_ = NULL;
				delete target_;
				target_ = NULL;
				size_--;
			} else {
				pop_end();
			}
		} else {
			std::cerr<<__PRETTY_FUNCTION__<<" : try to remove an element outside the list range"<<std::endl;
		}
	}
}

template<typename Type>
void List<Type>::pop_range(unsigned int const& a, unsigned int const& b){
	if(a==b){ pop(a); }
	if(a>b){ pop_range(b,a); }
	if(a<b){
		if(a!=0){
			Node* node_a(head_);
			for(unsigned int i(0);i<a-1;i++){ node_a = node_a->next_; }
			Node* tmp(node_a->next_);
			Node* node_b(node_a);
			for(unsigned int i(0);i<b-a-1;i++){ node_b = node_b->next_; }
			node_b->next_ = node_b->next_;
			node_b->next_ = NULL;
			delete tmp;
			size_ -= b-a;
		} else {
			pop_range(a+1,b);
			pop_start();
		}
	}
}
/*}*/

/*methods that modifies the list*/
/*{*/
template<typename Type>
void List<Type>::swap(unsigned int const& a, unsigned int const& b){
	if(a>b){ swap(b,a); }
	if(a<b){
		Node* node_a(head_);
		for(unsigned int i(0);i<a;i++){ node_a = node_a->next_; }
		Node* node_b(node_a);
		for(unsigned int i(0);i<b-a;i++){ node_b = node_b->next_; }
		std::swap(node_a->t_,node_b->t_);
	}
}

template<typename Type>
void List<Type>::handle_twin(std::shared_ptr<Type> t, std::function<void (Type&, Type&)> handling_function){
	handling_function(*target_->t_,*t);
}
/*}*/

/*methods that return something*/
/*{*/
template<typename Type>
bool List<Type>::find_sorted(std::shared_ptr<Type> t, std::function<unsigned int (Type const&, Type const&)> cmp){
	if(head_){//check if there is at least one element
		if(head_->next_){//check if there is at least two elements
			switch(cmp(*tail_->t_,*t)){ //check cmp(last,t)
				case 0://the last element is "bigger" than t
					{
						switch(cmp(*head_->t_,*t)){
							case 0:
								{
									target_ = NULL;
									return false;
								}break;
							case 1:
								{
									target_ = head_;
									unsigned int c(cmp(*target_->next_->t_,*t));
									while(c==1){
										target_ = target_->next_;
										c = cmp(*target_->next_->t_,*t);
									}
									if(c==0){ return false; }
									else {
										target_ = target_->next_;
										return true;
									}
								}break;
							case 2:
								{
									target_ = head_;
									return true;
								}
						}
					}break;
				case 1://the last element is "smaller" than t
					{
						target_ = tail_;
						return false;
					}break;
				case 2://the last element is equal to t
					{
						target_ = tail_;
						return true;
					}break;
			}
		} else {
			switch(cmp(*head_->t_,*t)){//check the unique element of the list
				case 0://the unique element is "bigger"  than t
					{
						target_ = NULL;
						return false;
					}break;
				case 1://the unique element is "smaller" than t
					{
						target_ = head_;
						return false;
					}break;
				case 2://the unique element is equal to t
					{
						target_ = head_;
						return true;
					}break;
			}
		}
		std::cerr<<__PRETTY_FUNCTION__<<" : unexpected value returned from cmp"<<std::endl;
		return false;
	} else {
		target_ = NULL;
		return false;
	}
}

template<typename Type>
bool List<Type>::target_next() const {
	if(target_ == NULL ){
		if(head_){ target_ = head_; }
		return target_;
	}
	target_ = target_->next_;
	return target_;
}

template<typename Type>
List<Type> List<Type>::sublist(unsigned int const& a, unsigned int const& b) const {
	target_ = head_;
	for(unsigned int i(1);i<a;i++){ target_ = target_->next_; }
	List<Type> tmp;
	for(unsigned int i(0);i<b-a;i++){
		target_ = target_->next_;
		tmp.add_end(std::make_shared<Type>(*target_->t_));
	}
	target_ = NULL;
	return tmp;
}
/*}*/
#endif
