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
  
  import { initializeApp } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-app.js";
  import { getDatabase, ref, set, get, onValue } from "https://www.gstatic.com/firebasejs/9.15.0/firebase-database.js";
  

  
  // 🔥 Inicijalizacija Firebase aplikacije
  const app = initializeApp(firebaseConfig);
  const db = getDatabase(app);
  
  // 📌 Funkcija za slanje podataka (gornja i donja granica temperature)
  function postaviGranice() {
      let donjaGranica = document.getElementById("donja").value;
      let gornjaGranica = document.getElementById("gornja").value;
  
      set(ref(db, "/postavke"), {
          donja: parseFloat(donjaGranica),
          gornja: parseFloat(gornjaGranica)
      }).then(() => {
          alert("✅ Podaci uspješno poslani na Firebase!");
      }).catch((error) => {
          alert("❌ Greška pri slanju: " + error);
      });
  }
  
  // 📌 Funkcija za dohvaćanje trenutne temperature sa ESP32
  function dohvatiTemperaturu() {
      const tempRef = ref(db, "/temperatura");
      onValue(tempRef, (snapshot) => {
          if (snapshot.exists()) {
              let temperatura = snapshot.val();
              document.getElementById("trenutnaTemp").innerText = `🌡️ Trenutna temperatura: ${temperatura} °C`;
          } else {
              document.getElementById("trenutnaTemp").innerText = "⚠️ Nema podataka!";
          }
      });
  }
  
  // ✅ Automatski dohvaća temperaturu u realnom vremenu
  dohvatiTemperaturu();
  
  // 🎯 Event listener za dugme
  document.getElementById("posalji").addEventListener("click", postaviGranice);