#include "rbtree.h"

#include <stdlib.h>

node_t *rotateLeft(rbtree *t, node_t *p);
node_t *rotateRight(rbtree *t, node_t *p);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK; //0은 블랙. nil 노드는 블랙.
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  //TODO: reclaim the tree nodes's memory
  node_t *pre, *temp;
  temp = t->root;

  while(temp != t->nil && temp != NULL){
    //왼쪽 자식이 있다면, 계속 자식을 향함
    if(temp->left != t->nil){
      temp = temp->left;
      continue;
    }

    if(temp->right != t->nil){
      temp = temp->right;
      continue;
    }
    //두 자식이 다 없다면
    pre = temp->parent;

    if(pre != t->nil && pre != NULL){
      if(pre->right == temp){
      //본인이 오른쪽
        pre->right = t->nil;
      }else{
        pre->left = t->nil;
      }
    }
    

    free(temp);
    temp = pre;
  }

  //nil 노드 삭제
  free(t->nil);

  //본인 삭제
  free(t);

  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *new_n, *pre, *temp, *Grand;

  pre = t->root;

  //새 노드 생성해둠
  new_n = malloc(sizeof(node_t));
  new_n->color = RBTREE_RED;
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
    new_n->parent = t->nil;
  }

  //노드 삽입 완료. 정렬해야함.

  temp = new_n;

  //레드가 연속되지 않는지 확인
  while(temp->parent != t->nil){ //루트까지 올라가면서 쭉 검사할거임...

    //본인이 빨강일 때
    if(temp->color == RBTREE_RED){
      //자식이 빨강이라면
      if(temp->left->color == RBTREE_RED){
        //자식 기준으로 다시 확인. 규칙 위배 확인.
        temp = temp->left;
        continue;
      }else if(temp->right->color == RBTREE_RED){
        temp = temp->right;
        continue;
      }else if (temp->parent->color == RBTREE_RED)
      {
        //부모의 색과 자신의 색이 모두 빨강이라면
        //삼촌 노드의 색 확인
        Grand = temp->parent->parent;


        if(Grand->left->color == Grand->right->color){
          //부모와 삼촌의 색이 같다면. 즉, 삼촌이 빨간색이면.
          //색을 바꿈
          Grand->color = RBTREE_RED;
          Grand->left->color = RBTREE_BLACK;
          Grand->right->color = RBTREE_BLACK;
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
              temp->left->parent = temp->parent;
              temp->parent->parent = temp;
              temp->left = temp->parent;
              temp->parent = Grand;
              temp = temp->left;
            }
            Grand = rotateRight(t, Grand);
            //돌리기만 했으므로 색 변경함
            Grand->color = RBTREE_RED;
            Grand->left->color = RBTREE_BLACK;


          }else{
            //부모가 조상의 오른쪽일때
            if(temp->parent->key >= temp->key){
              //일직선이 아님.
              Grand->right = temp;
              temp->parent->left = temp->right;
              temp->right->parent = temp->parent;
              temp->parent->parent = temp;
              temp->right = temp->parent;
              temp->parent = Grand;
              temp = temp->right;
            }
            Grand = rotateLeft(t, Grand);
            Grand->color = RBTREE_RED;
            Grand->right->color = RBTREE_BLACK;
            
          }
        }
      }
    }

    //정렬을 했다면... 위로 올라가면서 규칙 위배 다시 확인.
    temp = temp->parent;
  }

  //루트가 레드라면 블랙으로 바꿈
  if(t->root->color == RBTREE_RED){
    t->root->color = RBTREE_BLACK;
  }
  
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {             
  // TODO: implement find
  node_t *find;

  find = t->root;

  while(find != t->nil){ //리프노드 갈때까지 반복
    if(find->key == key){
      return find; //값 찾았다면 리턴
    }

    if(find->key>key){
      find = find->left; //찾는 값이 더 작음
    }else{ //찾는값이 더 큼
      find = find->right;
    }
  }

  return NULL; //못찾았으므로 null 리턴
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  //RB tree 중 최소 값을 가진 node pointer 반환

  //최소값은 무조건 제일 왼쪽에 있음
  node_t *min_node = t->root;
  while(min_node->left != t->nil){
    min_node = min_node->left;
  }

  return min_node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  //최대값을 가진 node pointer 반환
  
  //최대값은 무조건 제일 오른쪽에 있음
  node_t *max_node = t->root;
  while(max_node->right != t->nil){
    max_node = max_node->right;
  }

  return max_node;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  //삭제
  //RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
  color_t del_c = p->color;
  node_t *pre_d, *del_n;

  if(p == NULL || p == t->nil){
    return 0;
  }

  //삭제할 노드의 자식이 없거나, 하나만 있다면
  if(p->left == t->nil || p->right == t->nil){
    //해당 노드의 부모가 nil을 가리키게 해야함
    //부모의 왼쪽인지 오른쪽인지에 따라 다르게 처리

    if(p->left == t->nil){
      del_n = p->right;
    }else{
      del_n = p->left;
    }

    pre_d = p->parent;
    
    //부모노드랑 연결
    if(pre_d == t->nil){ //만약 루트노드면
      t->root = del_n;
    }else{
      if(p == pre_d->left){
        pre_d->left = del_n;
      }else{
        pre_d->right = del_n;
      }
    }
    
    if(del_n == t->nil){
      del_c = p->color;
    }else{
      del_n->parent = pre_d;
      del_c = del_n->color;
      del_n->color = p->color;
    }

    
  }else{
    //자식이 둘임
    //오른쪽에서 제일 작은 값 가져오기
    node_t *r_node = p->right;
    while(r_node->left != t->nil){
      r_node = r_node->left;
    }

    //삭제 노드 체크
    del_c = r_node->color;
    del_n = r_node->right;
    pre_d = r_node->parent;

    //대체
    if(p->parent == t->nil){ //삭제노드가 루트노드임
      t->root = r_node;
    }else{
      if(p == p->parent->left){
        p->parent->left = r_node;
      }else{
        p->parent->right = r_node;
      }
    }

    if (pre_d == p) {
      pre_d = r_node;
    } else {
      if (del_n != t->nil) {
        del_n->parent = pre_d;
      }
      pre_d->left = del_n;
      r_node->right = p->right;
      p->right->parent = r_node;
    }


    r_node->parent = p->parent;
    r_node->left = p->left;
    p->left->parent = r_node;
    r_node->color = p->color;
  }


  //규칙 확인
  //삭제된 색이 블랙일때만
  while(del_c == RBTREE_BLACK && del_n != t->root){
    //삭제된 위치에 새로 온 노드가 빨강이라면
    if(del_n->color == RBTREE_RED){
      //노드 색만 바꾸고 끝남.
      del_n->color = RBTREE_BLACK;
      break;
    }else{
      node_t *sibling;
      //아니라면.................
      //일단 본인이 nil인지 확인.
      if(del_n == t->nil){
      //값이 없다면, 부모의 다른쪽 자식이 값을 확인하고 그 기준으로 함.
        if(pre_d->left == t->nil){ //자신이 왼쪽임
          sibling = pre_d->right;
          if(sibling == t->nil){
            del_n = pre_d;
            pre_d = del_n->parent;
            continue;
          }
          //1. 형제의 컬러 확인
          if(sibling->color==RBTREE_RED){
            //만약 형제가 레드라면, 블랙으로 만들어야함.
            //부모와 형제의 색을 바꿈
            color_t c = sibling->color;
            sibling->color = pre_d->color;
            pre_d->color = c;

            //자신쪽으로 회전.
            pre_d = rotateLeft(t, pre_d);
            sibling = pre_d->right;
            if(sibling == t->nil){
            del_n = pre_d;
            pre_d = del_n->parent;
            continue;
          }
          }         
          //2. 형제의 자녀의 컬러 확인
          if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
            //둘 다 블랙임
            sibling->color = RBTREE_RED;
            del_n = pre_d;
            pre_d = del_n->parent;
            //부모 노드 기준으로 다시 해결
          }else{
            //둘 중 하나가 빨강임
            
            //목표자녀가 빨강아님
            if(sibling->right->color == RBTREE_BLACK){
              //회전
              sibling->color = RBTREE_RED;
              sibling->left->color = RBTREE_BLACK;
              sibling = rotateRight(t, sibling);
            }

            //이 뒤 1번 케이스에 따라 해결
            sibling->right->color = RBTREE_BLACK;
            sibling->color = pre_d->color;
            pre_d->color = RBTREE_BLACK;
            rotateLeft(t, pre_d);
            break;
          }
        }else{ //자신이 오른쪽임
          sibling = pre_d->left;
          if(sibling == t->nil){
              del_n = pre_d;
              pre_d = del_n->parent;
              continue;
          }
          if(sibling->color==RBTREE_RED){
            color_t c = sibling->color;
            sibling->color = pre_d->color;
            pre_d->color = c;
            pre_d = rotateRight(t, pre_d);
            sibling = pre_d->left;
            if(sibling == t->nil){
              del_n = pre_d;
              pre_d = del_n->parent;
              continue;
            }
          }  
          if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
            sibling->color = RBTREE_RED;
            del_n = pre_d;
            pre_d = del_n->parent;
          }else{
            if(sibling->left->color == RBTREE_BLACK){
              sibling->color = RBTREE_RED;
              sibling->right->color = RBTREE_BLACK;
              sibling = rotateLeft(t, sibling);
            }
            sibling->left->color = RBTREE_BLACK;
            sibling->color = pre_d->color;
            pre_d->color = RBTREE_BLACK;
            rotateRight(t, pre_d);
            break;
          } 
        }

      }else{
      //본인이 값이 있다면, 왼쪽인지 오른쪽인지 확인하고
        if(pre_d->left == del_n){ //자신이 왼쪽임
          sibling = pre_d->right;
          if(sibling == t->nil){
              del_n = pre_d;
              pre_d = del_n->parent;
              continue;
            }
          //1. 형제의 컬러 확인
          if(sibling->color==RBTREE_RED){
            //만약 형제가 레드라면, 블랙으로 만들어야함.
            //부모와 형제의 색을 바꿈
            color_t c = sibling->color;
            sibling->color = pre_d->color;
            pre_d->color = c;

            //자신쪽으로 회전.
            pre_d = rotateLeft(t, pre_d);
            sibling = pre_d->right;
            if(sibling == t->nil){
              del_n = pre_d;
              pre_d = del_n->parent;
              continue;
            }
          }         
          //2. 형제의 자녀의 컬러 확인
          if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
            //둘 다 블랙임
            sibling->color = RBTREE_RED;
            del_n = pre_d;
            pre_d = del_n->parent;
            //부모 노드 기준으로 다시 해결
          }else{
            //둘 중 하나가 빨강임
            
            //목표자녀가 빨강아님
            if(sibling->right->color == RBTREE_BLACK){
              //회전
              sibling->color = RBTREE_RED;
              sibling->left->color = RBTREE_BLACK;
              sibling = rotateRight(t, sibling);
            }

            //이 뒤 1번 케이스에 따라 해결
            sibling->right->color = RBTREE_BLACK;
            sibling->color = pre_d->color;
            pre_d->color = RBTREE_BLACK;
            rotateLeft(t, pre_d);
            break;
          }
        }else{ //자신이 오른쪽임
          sibling = pre_d->left;
          if(sibling == t->nil){
              del_n = pre_d;
              pre_d = del_n->parent;
              continue;
          }
          if(sibling->color==RBTREE_RED){
            color_t c = sibling->color;
            sibling->color = pre_d->color;
            pre_d->color = c;
            pre_d = rotateRight(t, pre_d);
            sibling = pre_d->left;
            if(sibling == t->nil){
              del_n = pre_d;
              pre_d = del_n->parent;
              continue;
            }
          }  
          if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK){
            sibling->color = RBTREE_RED;
            del_n = pre_d;
            pre_d = del_n->parent;
          }else{
            if(sibling->left->color == RBTREE_BLACK){
              sibling->color = RBTREE_RED;
              sibling->right->color = RBTREE_BLACK;
              sibling = rotateLeft(t, sibling);
            }
            sibling->left->color = RBTREE_BLACK;
            sibling->color = pre_d->color;
            pre_d->color = RBTREE_BLACK;
            rotateRight(t, pre_d);
            break;
          } 
        }
      }
    }
  }

   //루트가 레드라면 블랙으로 바꿈
  if(t->root->color == RBTREE_RED){
    t->root->color = RBTREE_BLACK;
  }

  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *temp = t->root;
  node_t *prev = t->nil;
  size_t i = 0;

  for (; temp != t->nil && i < n;) {
    if (prev == temp->parent) {
      // 노드에 처음 방문
      if (temp->left != t->nil) {
        prev = temp;
        temp = temp->left;
      } else {
        arr[i++] = temp->key;
        prev = temp;
        temp = (temp->right != t->nil) ? temp->right : temp->parent;
      }
    } else if (prev == temp->left) {
      // 왼쪽 자식에서 돌아옴
      arr[i++] = temp->key;
      prev = temp;
      temp = (temp->right != t->nil) ? temp->right : temp->parent;
    } else {
      // 오른쪽 자식에서 돌아옴
      prev = temp;
      temp = temp->parent;
    }
  }

  return 0;
}

//커스텀 함수

//오른쪽으로 회전
node_t *rotateRight(rbtree *t, node_t *p){
    node_t *left_n = p->left;
    
    if(p->left != t->nil){
      p->left = left_n->right;
      p->left->parent = p;
    }
    
    //부모 업데이트
    left_n->parent = p->parent;

    //부모 업데이트
    if(p->parent == t->nil){
      t->root = left_n;
    }else{
      if(p == p->parent->left){
        //조상의 왼쪽 자식임
        p->parent->left = left_n;
      }else{
        //조상의 오른쪽 자식임
        p->parent->right = left_n;
      }
    }
    
    left_n->right = p;
    p->parent = left_n;

    return left_n;
}

//왼쪽으로 회전
node_t *rotateLeft(rbtree *t, node_t *p){
    node_t *right_n = p->right;

    if(p->right != t->nil){
      p->right = right_n->left;
      p->right->parent = p;
    }

    //부모 업데이트
    right_n->parent = p->parent;

    //부모 업데이트
    if(p->parent == t->nil){
      t->root = right_n;
    }else{
      if(p == p->parent->left){
        //조상의 왼쪽 자식임
        p->parent->left = right_n;
      }else{
        //조상의 오른쪽 자식임
        p->parent->right = right_n;
      }
    }
    
    right_n->left = p;
    p->parent = right_n;

    return right_n;
}
