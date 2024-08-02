#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = malloc(sizeof(node_t));
  p->nil->color = 0; //0은 블랙. nil 노드는 블랙.
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *pre, *temp;
  pre = temp = t->root;

  while(pre != NULL){
    //자식 노드가 없을때 본인 삭제
    if(temp->left == NULL || temp->left == t->nil){
      if(temp->right == NULL || temp->right == t->nil){
        pre = temp->parent;
        free(temp);
        temp = pre;
      }
    }else{//자식 노드가 있을 때
      if(temp->left != t->nil){
        temp = temp->left;
      }else{
        temp = temp->right;
      }
    }
  }

  //nil 노드 삭제
  free(t->nil);

  //본인 삭제
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *new_n, *pre, *temp, *Grand;

  pre = t->root;

  //새 노드 생성해둠
  new_n = malloc(sizeof(node_t));
  new_n->color = 1;
  new_n->key = key;
  new_n->left = new_n->right = t->nil;

  while (pre != t->nil) //들어갈 자리 찾기.
  {
    if(pre->key >= key){
      //새로 들어오는 값이 지금 노드보다 작거나 같을때는 왼쪽으로 이동
      if(pre->left == t->nil){ //왼쪽이 비어있다면
        pre->left = new_n; //해당 값 넣음
        new_n->parent = pre;
        break;
      }
      pre = pre->left; //아니라면 다시 탐색
    }else{
      //새로 들어오는 값이 지금 노드보다 크다면 오른쪽
      if(pre->right == t->nil){ //오른쪽이 비어있다면
        pre->right = new_n; //해당 값 넣음
        new_n->parent = pre;
        break;
      }
      pre = pre->right; //아니라면 다시 탐색
    }
  } 

  if(t->root == t->nil){
    t->root = new_n;
    new_n->parent = NULL;
  }

  //노드 삽입 완료. 정렬해야함.

  temp = new_n;
  int item = 0;

  //레드가 연속되지 않는지 확인
  while(temp->parent != NULL){ //루트까지 올라가면서 쭉 검사할거임...

    //본인이 빨강일 때
    if(temp->color == 1){
      //자식이 빨강이라면
      if(temp->left->color == 1){
        //자식 기준으로 다시 확인. 규칙 위배 확인.
        temp = temp->left;
        continue;
      }else if(temp->right->color == 1){
        temp = temp->right;
        continue;
      }else if (temp->parent->color == 1)
      {
        //부모의 색과 자신의 색이 모두 빨강이라면
        //삼촌 노드의 색 확인
        Grand = temp->parent->parent;


        if(Grand->left->color == Grand->right->color){
          //부모와 삼촌의 색이 같다면. 즉, 삼촌이 빨간색이면.
          //색을 바꿈
          Grand->color = 1;
          Grand->left->color = 0;
          Grand->right->color = 0;
        }else{
          //삼촌이 검은색일때
          //재배치해야함

          //일직선인지 확인
          if(Grand->key >= temp->parent->key){
            //부모가 조상의 왼쪽일때
            if(temp->parent->key < temp->key){
              //일직선이 아니니까, 본인과 부모의 위치를 바꿈
              Grand->left = temp;
              temp->parent->right = temp->left;
              temp->parent->parent = temp;
              temp->left = temp->parent;
              temp->parent = Grand;
              temp = temp->left;
            }
              //자신도 부모의 왼쪽이라면
              //일직선이니까.... 부모와 조상의 값을 바꿈
              item = Grand->key;
              Grand->key = temp->parent->key;
              temp->parent->key = item;

              //조상이 된 부모가 다시 자신을 보게 하고, 부모 위치에 온 조상을 부모의 왼쪽으로 넘김...
              Grand->left = temp;
              temp->parent->left = temp->parent->right;
              temp->parent->right = Grand->right;
              Grand->right = temp->parent;
              temp->parent = Grand;
          }else{
            //부모가 조상의 오른쪽일때
            if(temp->parent->key >= temp->key){
              //일직선이 아님.
              Grand->right = temp;
              temp->parent->left = temp->right;
              temp->parent->parent = temp;
              temp->right = temp->parent;
              temp->parent = Grand;
              temp = temp->right;
            }
              item = Grand->key;
              Grand->key = temp->parent->key;
              temp->parent->key = item;
              Grand->right = temp;
              temp->parent->right = temp->parent->left;
              temp->parent->left = Grand->left;
              Grand->left = temp->parent;
              temp->parent = Grand;
            
          }
        }
      }
    }

    //정렬을 했다면... 위로 올라가면서 규칙 위배 다시 확인.
    temp = temp->parent;
  }

  //루트가 레드라면 블랙으로 바꿈
  if(t->root->color == 1){
    t->root->color = 0;
  }
  
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  //삭제
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
