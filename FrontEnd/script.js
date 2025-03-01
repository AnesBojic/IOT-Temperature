// script.js



// Firebase konfiguracija
const firebaseConfig = {
    apiKey: "AIzaSyBvuf6p0uC4NFgmkidbWDujCCwWcXiAs9A",
    authDomain: "iot-temp-75625.firebaseapp.com",
    databaseURL: "https://iot-temp-75625-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "iot-temp-75625",
    storageBucket: "iot-temp-75625.firebasestorage.app",
    messagingSenderId: "958701212786",
    appId: "1:958701212786:web:3cb1ba046cfae197aa5eeb"
  };
  
  // Inicijalizacija Firebase
  const app = firebase.initializeApp(firebaseConfig);
  const database = firebase.database();
  
  // Funkcija za spremanje granica temperature
  function saveLimits() {
    const minTemp = document.getElementById("minTemp").value;
    const maxTemp = document.getElementById("maxTemp").value;
  
    if (minTemp && maxTemp) {
      // Upisivanje granica u Firebase
      database.ref("limits").set({
        min: parseFloat(minTemp),
        max: parseFloat(maxTemp)
      });
      
      // Obaveštenje korisnika
      document.getElementById("status").innerHTML = "✅ Temperature limits saved!";
      document.getElementById("status").style.color = "green";
    } else {
      document.getElementById("status").innerHTML = "⚠️ Please enter both values.";
      document.getElementById("status").style.color = "red";
    }
  }