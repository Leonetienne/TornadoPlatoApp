# Copy repository readme here to be used as a cover page
tail ../readme.md -n +2 > index.md

# Run doxygen
doxygen doxyfig

# Cleanup index.md
rm -f index.md

