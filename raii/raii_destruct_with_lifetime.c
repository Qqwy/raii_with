#include "raii.h"

/** Helper function that properly destructs the resource stored in a `raii_lifetime_elem` struct. */
void destruct_raii_lifetime(struct raii_lifetime_elem elem) {
  elem.destructor(elem.resource);
}
