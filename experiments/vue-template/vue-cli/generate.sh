#!/bin/sh
cd /vue
npm install -g @vue/cli || exit 1
vue create $VUE_APP
cp dockerdev.sh $VUE_APP/
chown -R $DOCKER_USER: /vue
