BRANCH="master"

echo -e "Entering  set_tags.sh.\n"
echo -e "TRAVIS_PULL_REQUEST = " $TRAVIS_PULL_REQUEST
echo -e "TRAVIS_BRANCH = " $TRAVIS_BRANCH
echo -e "TRAVIS_TAG = " $TRAVIS_AG
echo -e "BRANCH = " $BRANCH

if [ "$TRAVIS_BRANCH" = "$BRANCH" ]; then
  if [ "$TRAVIS_PULL_REQUEST" = false ]; then
  	if [ -z "$TRAVIS_TAG" ]; then
	    echo -e "Starting to tag commit.\n"
            pwd
	    
	    git config --global user.email ${GIT_EMAIL}
	    git config --global user.name  ${GIT_NAME}

            echo -e "TRAVIS_BUILD_NUMBER = " $TRAVIS_BUILD_NUMBER

	    # Add tag and push to master.
	    git tag -a v${TRAVIS_BUILD_NUMBER} -m "Travis build $TRAVIS_BUILD_NUMBER pushed a tag."
	    git push origin --tags
	    git fetch origin

	    echo -e "Done magic with tags.\n"
	fi
  fi
fi
