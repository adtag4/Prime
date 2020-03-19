cmake_minimum_required(VERSION 3.10)
project(Prime)

set(CMAKE_CXX_STANDARD 17)


add_executable(Node
        src/node_main.cpp
        src/algorithm/algorithm.cpp
        src/algorithm/ecm.cpp
        src/algorithm/pollard.cpp
        src/algorithm/quadratic.cpp
        src/pfp/localUser.cpp
        src/pfp/localNode.cpp
        src/pfp/remoteUser.cpp
        src/pfp/remoteNode.cpp
        src/pfp/pfp.cpp
        src/QS/linear-algebra/gauss-elimination.cpp
        src/QS/linear-algebra/matrix.cpp
        src/QS/eratosthenes.cpp
        src/QS/gmp-patch.c
        src/QS/math-utils.C
        src/QS/QS-parallel.cpp
        src/QS/QS.cpp
        src/QS/smooth-number.cpp
        src/QS/smooth-base.cpp
        src/QS/utils.cpp
        include/algorithm/algorithm.h
        include/algorithm/ecm.h
        include/algorithm/pollard.h
        include/algorithm/quadratic.h
        include/pfp/localUser.h
        include/pfp/localNode.h
        include/pfp/remoteUser.h
        include/pfp/remoteNode.h
        include/pfp/pfp.h
        include/QS/linear-algebra/gauss-elimination.h
        include/QS/linear-algebra/matrix.h
        include/QS/eratosthenes.h
        include/QS/gmp-patch.h
        include/QS/math-utils.h
        include/QS/QS-parallel.h
        include/QS/QS.h
        include/QS/smooth-number.h
        include/QS/smooth-base.h
        include/QS/utils.h
        )

add_executable(User
        src/user_main.cpp
        src/algorithm/algorithm.cpp
        src/algorithm/ecm.cpp
        src/algorithm/pollard.cpp
        src/algorithm/quadratic.cpp
        src/pfp/localUser.cpp
        src/pfp/localNode.cpp
        src/pfp/remoteUser.cpp
        src/pfp/remoteNode.cpp
        src/pfp/pfp.cpp
        src/QS/linear-algebra/gauss-elimination.cpp
        src/QS/linear-algebra/matrix.cpp
        src/QS/eratosthenes.cpp
        src/QS/gmp-patch.c
        src/QS/math-utils.C
        src/QS/QS-parallel.cpp
        src/QS/QS.cpp
        src/QS/smooth-number.cpp
        src/QS/smooth-base.cpp
        src/QS/utils.cpp
        include/algorithm/algorithm.h
        include/algorithm/ecm.h
        include/algorithm/pollard.h
        include/algorithm/quadratic.h
        include/pfp/localUser.h
        include/pfp/localNode.h
        include/pfp/remoteUser.h
        include/pfp/remoteNode.h
        include/pfp/pfp.h
        include/QS/linear-algebra/gauss-elimination.h
        include/QS/linear-algebra/matrix.h
        include/QS/eratosthenes.h
        include/QS/gmp-patch.h
        include/QS/math-utils.h
        include/QS/QS-parallel.h
        include/QS/QS.h
        include/QS/smooth-number.h
        include/QS/smooth-base.h
        include/QS/utils.h
        )


target_include_directories(Node PRIVATE src include src/algorithm src/pfp
        src/QS src/QS/linear-algebra)
target_include_directories(User PRIVATE src include src/algorithm src/pfp
        src/QS src/QS/linear-algebra)


TARGET_LINK_LIBRARIES( Node pthread gmp gmpxx mpfr omp)
TARGET_LINK_LIBRARIES( User pthread gmp gmpxx mpfr omp)
