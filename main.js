window.onload = () => {
    const chart = document.getElementById("day-graph-canvas");
    const ctx = chart.getContext('2d');
    let gradient = ctx.createLinearGradient(0, 0, 0, 350);
    gradient.addColorStop(0, 'rgb(0, 187, 240)');
    gradient.addColorStop(1, 'rgba(246, 246, 246, 1)');

    const labels_temp = ["0:00", "0:30", "1:00", "1:30", "2:00", "2:30", "3:00", "3:30", "4:00", "4:30", "5:00", "5:30", "6:00", "6:30", "7:00", "7:30", "8:00", "8:30", "9:00", "9:30", "10:00", "10:30", "11:00", "11:30", "12:00", "12:30", "13:00", "13:30", "14:00", "14:30", "15:00", "15:30", "16:00", "16:30", "17:00", "17:30", "18:00", "18:30", "19:00", "19:30", "20:00", "20:30", "21:00", "21:30", "22:00", "22:30", "23:00", "23:30"];
    let d = new Date();
    console.log(d.getHours());
    let labels = [];
    for (let i = 0; i < labels_temp.length; i++) {
        console.log(labels_temp[0].substring(0, labels_temp[0].indexOf(":")), d.getHours(), labels_temp[i].substring(labels_temp[i].indexOf(":") + 1, labels_temp[i].length), d.getMinutes())
        if (labels_temp[i].substring(0, labels_temp[i].indexOf(":")) == d.getHours()) {
            labels.push(labels_temp[i]);
            if (labels_temp[i].substring(labels_temp[i].indexOf(":") + 1, labels_temp[i].length) <= d.getMinutes()) {
                labels.push(labels_temp[i + 1])
            }
            break;
        } else {
            labels.push(labels_temp[i]);
        }
    }
    labels = labels.slice(-12);
    console.log(labels);
    console.log(labels_temp);
    let dataset = [];
    const data = {
        labels: labels,
        datasets: [{
            label: 'Temperature',
            data: dataset,
            borderColor: "rgb(92, 156, 230)",
            backgroundColor: gradient,
            fill: true,
            cubicInterpolationMode: 'monotone',
        }, ]
    };
    const config = {
        type: 'line',
        data: data,
        options: {
            responsive: true,
            plugins: {
                legend: {
                    display: false,
                    position: 'top',
                    labels: {
                        color: "#fff",
                    }
                },
                tooltip: {
                    callbacks: {
                        label: function (tooltipItem, data) {
                            return tooltipItem.formattedValue + "°C";
                        }
                    }
                }
            },
            scales: {
                x: {
                    border: {
                        display: false
                    },
                    grid: {},
                    ticks: {
                        stepSize: 20,
                        callback: function (value, index, ticks) {
                            return (window.innerWidth <= 500) ? ((index % 2 == 0) ? labels[value] : "") : labels[value];
                        },
                        padding: 0,
                    }
                },
                y: {
                    position: 'top',
                    border: {
                        display: false
                    },
                    grid: {
                        lineWidth: context => context.tick.value == 0 ? 2 : 0
                    },
                    ticks: {
                        callback: function (value, index, ticks) {
                            return (index % 2 == 0) ? value + "°C" : "";
                        },
                    }
                }
            },
        },
    };

    let temp_chart = new Chart(ctx, config);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("tempActual").innerHTML = this.responseText;
            }
        }
        xhttp.open("GET", "/temperaturec", true);
        xhttp.send();
    }, 2000);

    function get_temp() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                ds = this.responseText;
                dataset = ds.split(",");
                temp_chart.data.datasets[0].data = dataset;
                temp_chart.update();
                setTimeout(get_temp, 1000 * 60)
            }
        };
        xhttp.open("GET", "/temperatures", true);
        xhttp.send();
    };

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("water").innerHTML = this.responseText;
                if (this.responseText == "Pumping") {
                    document.getElementById("water").parentElement.classList.add("active");
                } else {
                    document.getElementById("water").parentElement.classList.remove("active");
                }
            }
        };
        xhttp.open("GET", "/pumpc", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("heat_mat").innerHTML = this.responseText;
                if (this.responseText == "Heating") {
                    document.getElementById("heat_mat").parentElement.classList.add("active");
                } else {
                    document.getElementById("heat_mat").parentElement.classList.remove("active");
                }
            }
        };
        xhttp.open("GET", "/heat_mat", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("voltage").innerHTML = this.responseText + "V";
            }
        };
        xhttp.open("GET", "/voltage", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("current").innerHTML = this.responseText + "A";
            }
        };
        xhttp.open("GET", "/current", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("cost").innerHTML = "£" + this.responseText;
            }
        };
        xhttp.open("GET", "/cost_per_day", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("total_cost").innerHTML = "£" + this.responseText;
            }
        };
        xhttp.open("GET", "/total_cost", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("power").innerHTML = this.responseText + "W";
            }
        };
        xhttp.open("GET", "/power", true);
        xhttp.send();
    }, 2000);

    setInterval(() => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("pf").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/pf", true);
        xhttp.send();
    }, 2000);

    function add(accumulator, a) {
        return accumulator + a;
    }

    const weather_to_div = {
        "Clouds": "cloudy",
        "Rain": "breezy",
        "Drizzle": "breezy",
        "Snow": "stormy",
        "Thunderstorm": "stormy",
        "Clear": "hot"
    }

    let total_cost = 0;
    let power = [];

    function to_time(timestamp) {
        let date = new Date(timestamp * 1000);
        date = date.toLocaleTimeString('en-UK');
        let hour = date.split(":")[0];
        let amPm = date.split(" ")[1];
        let seconds = date.split(":")[2].replace(amPm, '');
        let noAmPm = date.replace(amPm, '');
        let noAmPmSeconds = noAmPm.replace(":" + seconds, '');
        return noAmPmSeconds;
    }

    function update_temp() {
        document.getElementsByClassName("top-labels")[0].innerHTML = "";
        for (let i = 0; i < dataset.length; i++) {
            if (dataset[i] == 0) continue;
            let temp = dataset[i];
            var innerDiv = document.createElement('h2');
            innerDiv.innerHTML = temp;
            var sup = document.createElement("sup");
            sup.innerHTML = "&deg;C"
            innerDiv.appendChild(sup);
            document.getElementsByClassName("top-labels")[0].appendChild(innerDiv)
        }
    }

    function convertDateToString(date) {
        return moment(date).format("dddd, MMMM Do YYYY")
    }

    document.getElementById("date").innerHTML = convertDateToString(new Date());

    function update_weather() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                const weather = JSON.parse(this.responseText);
                const current_sky = weather["weather"][0]["main"];
                document.getElementById("current_forecast").innerHTML = current_sky;
                console.log(weather_to_div, current_sky, document.getElementsByClassName(weather_to_div[current_sky]));
                document.getElementsByClassName("hot")[0].style.display = "none"
                document.getElementsByClassName("cloudy")[0].style.display = "none"
                document.getElementsByClassName("stormy")[0].style.display = "none"
                document.getElementsByClassName("breezy")[0].style.display = "none"
                document.getElementsByClassName("night")[0].style.display = "none"
                document.getElementsByClassName(weather_to_div[current_sky])[0].style.display = "block"
                document.getElementById("sunrise").innerHTML = to_time(weather["sys"]["sunrise"]);
                document.getElementById("sunset").innerHTML = to_time(weather["sys"]["sunset"]);
                console.log(weather["sys"]["sunrise"], weather["sys"]["sunset"]);
                document.getElementById("temp").innerHTML = (weather["main"]["temp"] - 273.15).toFixed(0)
                setTimeout(update_weather, 1000 * 60 * 30);
            }
        };
        xhttp.open("GET", "https://api.openweathermap.org/data/2.5/weather?lat=51.85242&lon=1.225550&appid=YOUR APP ID HERE", true);
        xhttp.send();
    }

    get_temp();
    update_weather();
}

function togglePump() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {}
    };
    xhttp.open("GET", "/pumpctog", true);
    xhttp.send();
}

function toggleHeatMat() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {}
    };
    xhttp.open("GET", "/heat_mattog", true);
    xhttp.send();
}
