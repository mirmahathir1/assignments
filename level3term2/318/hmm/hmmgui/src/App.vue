<template>
    <div class="centered">
        <b-navbar toggleable="lg" type="dark" variant="info">
            <b-navbar-brand href="#">{{text}}</b-navbar-brand>
        </b-navbar>


        <div v-for="i in 9" :key="i" v-if="probabilities!==null">
            <b-button v-if="probabilities[i]" squared v-for="j in 9" :key="j" :style="'width: 60px;height:60px; background-color: rgb('+probabilities[i][j]*2000+',0,0)'"
                      :variant="'outline'" @click="buttonClicked(i,j)"
            >
                <span class="text-white">{{probabilities[i][j]}}</span>
            </b-button>
        </div>
    </div>
</template>

<script>
import HelloWorld from './components/HelloWorld.vue'
import axios from 'axios'

export default {
    name: 'App',

    data(){
        return{
            text:"ghost not found",
            probabilities:[
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
                [0,0,0,0,0,0,0,0,0],
            ]
        }
    },

    components: {
        HelloWorld
    },
    mounted() {
        axios.get('http://127.0.0.1:6789/restart/')
        .then((res)=>{
            let arr = res.data.split("_");

            const probabilities = [];

            while (arr.length) probabilities.push(arr.splice(0, 9));

            this.probabilities = probabilities;
            console.log(probabilities);
        }).catch((err)=>{
            console.log("error happened");
        })
    },
    methods: {
        buttonClicked(row, col) {
            axios.get('http://127.0.0.1:6789/sense/' + row + "_" + col)
                .then((res) => {
                    if(res.data==="found"){
                        this.text="Ghost was found!!!!"
                    }

                    let arr = res.data.split("_");

                    const probabilities = [];

                    while (arr.length) probabilities.push(arr.splice(0, 9));

                    this.probabilities = probabilities;
                    console.log(probabilities);
                }).catch((err) => {
                console.log("error happened");
            })
        }
    }
}
</script>

<style>
#app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    margin-top: 60px;
}
</style>
