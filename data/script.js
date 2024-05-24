document.addEventListener('DOMContentLoaded', () => {
    // Obtener el elemento home-lock-unlock por su ID
    const lockUnlockElement = document.getElementById('lockUnlockElement');

    // Función para cambiar el color del elemento cuando se bloquea el dispositivo
    function setLocked() {
        lockUnlockElement.classList.add('locked');
    }

    // Función para cambiar el color del elemento cuando se desbloquea el dispositivo
    function setUnlocked() {
        lockUnlockElement.classList.remove('locked');
    }

    // Función para enviar una solicitud HTTP GET al servidor ESP32 para bloquear el dispositivo
    function sendLockRequest() {
        fetch('/setLocked')
            .then(response => {
                if (response.ok) {
                    setLocked();
                }
            })
            .catch(error => console.error('Error:', error));
    }

    // Función para enviar una solicitud HTTP GET al servidor ESP32 para desbloquear el dispositivo
    function sendUnlockRequest() {
        fetch('/setUnlocked')
            .then(response => {
                if (response.ok) {
                    setUnlocked();
                }
            })
            .catch(error => console.error('Error:', error));
    }

    // Agregar eventos de clic a los botones LOCK y UNLOCK para enviar solicitudes al servidor ESP32
    document.getElementById('lockButton').addEventListener('click', sendLockRequest);
    document.getElementById('unlockButton').addEventListener('click', sendUnlockRequest);

    // Función para obtener datos del sensor desde el servidor ESP32
    async function fetchSensorData() {
        try {
            const response = await fetch('/data');
            const data = await response.json();
            const roundedTemperature = Math.round(data.temperature);

            // Actualiza los elementos HTML con los nuevos valores
            document.getElementById('temperature').textContent = `${roundedTemperature}º`;
            document.getElementById('humidity').textContent = `${data.humidity}%`;
        } catch (error) {
            console.error('Error fetching sensor data:', error);
        }
    }

    // Llama a la función para obtener datos cada 5 segundos
    setInterval(fetchSensorData, 5000);

    // Llama a la función inmediatamente para obtener datos al cargar la página
    fetchSensorData();
});