<template>
  <v-container pa-2>
    <v-layout text-xs-center wrap>
      <v-flex xs12>
        <h1 class="display-3 font-weight-bold">Setting</h1>
      </v-flex>

      <v-flex xs12 pb-2 pt-3>
        <v-btn
          large
          block
          outline
          class="py-2 my-0 px-2 text-none"
          style="height: auto;"
          @click.stop="dialog = true"
        >
          <h2 class="display-1 font-weight-bold">Time :</h2>
          <v-spacer/>
          <h2 class="display-1 font-weight-medium">{{min}}:00</h2>
          <h2 class="display-1 font-weight-bold">&nbsp;min</h2>
        </v-btn>
      </v-flex>

      <v-flex xs12 py-2>
        <v-btn
          large
          block
          outline
          class="py-2 my-0 px-2 text-none"
          style="height: auto;"
          @click.stop="dialog2 = true"
        >
          <h2 class="display-1 font-weight-bold">Pre-temp :</h2>
          <v-spacer/>
          <h2 class="display-1 font-weight-medium">{{temp}}</h2>
          <h2 class="display-1 font-weight-bold">&nbsp;°C</h2>
        </v-btn>
      </v-flex>

      <v-flex xs12 text-xs-center mt-3>
        <div class="my-3">
          <v-btn
            large
            block
            class="display-1 py-1 font-weight-medium"
            style="height: auto"
            color="error"
            disabled
          >WATER FULL</v-btn>
        </div>
        <div class="my-3">
          <v-btn
            large
            block
            class="display-1 py-1 font-weight-medium"
            style="height: auto"
            color="success"
            disabled
          >READY</v-btn>
        </div>
      </v-flex>

      <v-flex xs12 text-xs-center mt-2>
        <v-layout justify-center>
          <v-flex xs6>
            <v-btn
              large
              class="display-1 px-3 py-2"
              style="height: auto"
              color="success"
              disabled
            >START</v-btn>
          </v-flex>
          <v-flex xs6>
            <v-btn
              large
              class="display-1 px-3 py-2"
              style="height: auto"
              color="error"
              disabled
            >STOP</v-btn>
          </v-flex>
        </v-layout>
      </v-flex>
    </v-layout>

    <v-dialog v-model="dialog" width="70%">
      <v-card class="text-xs-center">
        <v-card-text>
          <h2 class="display-2 font-weight-bold">Time</h2>
        </v-card-text>

        <v-card-text>
          <v-layout justify-center row wrap>
            <v-flex xs8>
              <v-btn
                large
                class="display-1 px-2 py-2"
                style="height: auto"
                color="primary"
                @click="min++"
              >+</v-btn>
            </v-flex>
            <v-flex xs4></v-flex>
            <v-flex xs8>
              <h2 class="display-1 font-weight-medium my-2">{{min}}:00</h2>
            </v-flex>
            <v-flex xs4>
              <h2 class="display-1 font-weight-medium my-2">min</h2>
            </v-flex>
            <v-flex xs8>
              <v-btn
                large
                class="display-1 px-2 py-2"
                style="height: auto"
                color="primary"
                @click="min--"
              >-</v-btn>
            </v-flex>
            <v-flex xs4></v-flex>
          </v-layout>
        </v-card-text>
      </v-card>
    </v-dialog>

    <v-dialog v-model="dialog2" width="70%">
      <v-card class="text-xs-center">
        <v-card-text>
          <h2 class="display-2 font-weight-bold">Pre-temp</h2>
        </v-card-text>

        <v-card-text>
          <v-layout justify-center row wrap>
            <v-flex xs8>
              <v-btn
                large
                class="display-1 px-2 py-2"
                style="height: auto"
                color="primary"
                @click="temp++"
              >+</v-btn>
            </v-flex>
            <v-flex xs4></v-flex>
            <v-flex xs8>
              <h2 class="display-1 font-weight-medium my-2">{{temp}}</h2>
            </v-flex>
            <v-flex xs4>
              <h2 class="display-1 font-weight-medium my-2 pr-2">°C</h2>
            </v-flex>
            <v-flex xs8>
              <v-btn
                large
                class="display-1 px-2 py-2"
                style="height: auto"
                color="primary"
                @click="temp--"
              >-</v-btn>
            </v-flex>
            <v-flex xs4></v-flex>
          </v-layout>
        </v-card-text>
      </v-card>
    </v-dialog>
  </v-container>
</template>

<script>
export default {
  data() {
    return {
      dialog: false,
      dialog2: false,
      temp: 0,
      min: 10
    };
  }
};
</script>