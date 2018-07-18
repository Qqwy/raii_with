#include "raii.ih"

void destruct_raii_with_lifetime(struct raii_with_lifetime_elem elem) {
  elem.destructor(elem.resource);
}
