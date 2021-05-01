echo "Fixing " $1
find "./build/$1/" -mindepth 1 -maxdepth 1 -name '*' -type f | egrep '*\.(html)$' | xargs -I '{}' sed -i 's/href="/href="\/'$1'\//g' {}
find "./build/$1/" -mindepth 1 -maxdepth 1 -name '*' -type f | egrep '*\.(html)$' | xargs -I '{}' sed -i 's/src="/src="\/'$1'\//g' {}
find "./build/$1/" -mindepth 1 -maxdepth 1 -name '*' -type f | egrep '*\.(js)$' | xargs -I '{}' sed -i 's/href="/href="\/'$1'\//g' {}
find "./build/$1/" -mindepth 1 -maxdepth 1 -name '*' -type f | egrep '*\.(js)$' | xargs -I '{}' sed -i 's/src="/src="\/'$1'\//g' {}
find "./build/$1/" -mindepth 1 -maxdepth 1 -name '*' -type f | egrep '*\.(css)$' | xargs -I '{}' sed -i 's/url('\''/url('\''\/'$1'\//g' {}
