<template>
    <div>
        <button @click="viewing = !viewing">{{ viewing ? "Edit" : "View" }}</button>
        <span v-show="!viewing">
            <button @click="addBold"><b>b</b></button>
            <button @click="addItalics"><i>i</i></button>
            ...more tools here
        </span>
        <hr>
        <textarea v-if="!viewing" v-model="adocSource" ref="ta"></textarea> 
        <Viewer v-else :adocSource="adocSource"/>
    </div>
</template>


<script>
    import Viewer from "./Viewer.vue";
    export default {
        name: "App",
        components: {Viewer},
        data() {
            return {
                adocSource: "",
                adocRendered: "",
                viewing: false
            }
        },
        methods: {
            addBold() {
                this.wrapSelection("*");
            },
            addItalics() {
                this.wrapSelection("_");
            },
            /**
             * wrap text between start and end index in the given symbol
             */
            wrapSelection(symbol) {
                let selStart = this.$refs.ta.selectionStart-1;
                let selEnd = this.$refs.ta.selectionEnd-1;
                if (selStart === this.adocSource.length-1) {
                    this.adocSource += symbol + symbol;
                    // TODO: Set selectionStart between the two stars
                    return;
                }
                let pre = this.adocSource.substr(0, selStart+1);
                let selected = this.adocSource.substr(selStart+1, selEnd-selStart)
                let post = this.adocSource.substr(selEnd+1);
                this.adocSource = pre + symbol + selected + symbol + post;
            }
        }
    }
</script>

<style scoped>
    textarea {
        width: 100%;
        height: 100%;
        display: block;
        border: 0;
        /*outline: none;*/
    }
</style>
