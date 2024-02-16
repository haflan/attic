<template>
    <div class="main-wrapper" v-html="adocRendered"></div>
</template>


<script>
    const adoc = require("asciidoctor")();
    export default {
        name: "Viewer",
        props: {
            adocSource: String
        },
        data() {
            return {
                adocRendered: ""
            }
        },
        mounted() {
            this.adocRendered = adoc.convert(this.adocSource, {safe: 'safe'});
            if (!window.MathJax) {
                return;
            }
            this.$nextTick().then(() => {
                window.MathJax.Hub.Typeset();
            // Thanks: 
            // - https://docs.mathjax.org/en/v2.7-latest/advanced/typeset.html
            // - https://stackoverflow.com/questions/52636554/mathjax-vue-not-rerendering-equations
            });
        }
    }
</script>


<style scoped>
</style>
