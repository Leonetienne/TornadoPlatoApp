# Run doxygen
sudo doxygen tornado_doxyfig
sudo doxygen plato_doxyfig

# Fix links
echo "Fixing links..."
./doxyroles/doxyfix.sh "Tornado"
./doxyroles/doxyfix.sh "Plato"