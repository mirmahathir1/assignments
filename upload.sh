echo "Enter commit message:"
read message
git add .
git commit -am "$message"
git push origin master