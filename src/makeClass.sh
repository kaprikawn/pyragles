#!/bin/bash

className=$1

if [[ -z $className ]]; then
  echo "No class provided"
  exit 1
fi

cat <<EOF > ${className}.hpp
#ifndef ${className^^}_HPP
#define ${className^^}_HPP
class $className {
  
  private :
    
  public :
    ${className}();
    ~${className}();
    
    
};
    
#endif //${className^^}_HPP
EOF
cat <<EOF > ${className}.cpp
#include "${className}.hpp"
${className}::${className}() {
  
}
${className}::~${className}() {
  
}
EOF

exit 0
