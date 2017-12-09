#include <stdio.h>
#include <stdlib.h>
#include "commom.h"
#include <iostream>

using namespace std;

void traversal(page *node)
{
  int i;
  if (node)
  {
    for (i = 0; i < node->keyCount; i++)
    {
      traversal(node->children[i]);
      cout << node->keys[i + 1] << ' ';
    }
    traversal(node->children[i]);
  }
}