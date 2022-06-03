#currentBranch=$(git symbolic-ref --short -q HEAD)
#
#remoteRepo=github_sdk
#isRelease=$2 #是否正式发布，测试发布时发布到suguiming的个人目录下
#
#
#git tag -d $(git tag)
#  
#git branch -D main
#
#git subtree split --prefix=Plugins --branch main
#
#git remote rm $remoteRepo
#
#
#if [ $isRelease = true ]; then
#   git remote add $remoteRepo git@github.com:xd-platform/XDGSDK-UE4.git
#else
#   git remote add $remoteRepo git@github.com:suguiming/XDGSDK-UE4.git
#fi;
#
#
#git tag $1
#
#git checkout main --force
#
#git fetch --unshallow main
#
#git push $remoteRepo main --force --tags
#
#git checkout $currentBranch --force
