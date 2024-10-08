# Build the include files
echo "Building the include files..."
./deggl -i ../Eule/*.cpp -o Eule

# Verify that they compile cleanly
# For simplicity, we'll test without intrisics enabled
echo "Verifying that they compile"
g++ Eule.cpp -c -S -std=c++17 -o - -Wall -Wextra -Wpedantic -mavx -D _EULE_NO_INTRINSICS_ > /dev/null
