#!/bin/bash

className=$1
classIdentifer=${className^l}

if [[ -z $className ]]; then
  echo "No class provided"
  exit 1
fi

cat <<EOF > ${className}.hpp
#ifndef ${className^^}_HPP
#define ${className^^}_HPP

class $classIdentifer {
  
  private :
    
  public :
    ${classIdentifer}();
    ~${classIdentifer}();
    
    
};
    
#endif //${className^^}_HPP
EOF

cat <<EOF > ${className}.cpp
#include "${className}.hpp"
#include <iostream>

${classIdentifer}::${classIdentifer}() {
  
}

${classIdentifer}::~${classIdentifer}() {
  
}

EOF

exit 0
