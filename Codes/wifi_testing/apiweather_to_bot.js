const TelegramBot = require('node-telegram-bot-api');
const token = 'TOKEN';
const bot = new TelegramBot(token, { polling: true });
const weather = require('weather-js');

bot.onText(/^\/clima (.+)/, function(msg, match){
    var chatId = msg.chat.id;
    var ciudad = match[1];

    var opciones = {
        search: ciudad, // lugar es la ciudad que el usuario introduce
        degreeType: 'C', // Celsius
        lang: 'es-ES' // Lenguaje en el que devolverá los datos
    }

    weather.find(opciones, function(err, result){

        if (err){ // Si ocurre algun error...
            console.log(err); // ... nos lo muestra en pantalla

        } else {
            console.log(result[0]); // Visualizamos el primer resultado del array
            
            bot.sendMessage(chatId, "Lugar: " + result[0].location.name +
            "\n\nTemperatura: " + result[0].current.temperature + "ºC\n" +
            "Visibilidad: " + result[0].current.skytext + "\n" +
            "Humedad: " + result[0].current.humidity + "%\n" +
            "Dirección del viento: " + result[0].current.winddisplay + "\n"
            ,{parse_mode: 'Markdown'});

        }
    })
});