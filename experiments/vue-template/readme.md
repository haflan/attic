In this dir: Two versions of Vue boilerplate / setup.
Both run `npm` in a Docker container, to avoid polluting the local file system ;)

- `vue-cli`: For generating boilerplate for future dev with `vue-cli`. 
             Simply run `setup.sh` to generate a fresh Vue app .
- `webpack`: Can be used as is, just run dockerdev.sh. Uses `webpack` _instead
             of_ `vue-cli`
