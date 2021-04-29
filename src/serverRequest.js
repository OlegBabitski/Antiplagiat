document.addEventListener("DOMContentLoaded", function () {
    const sendButton = document.getElementById('send') // button send
    const backButton = document.getElementById('back') // button back

    sendButton.addEventListener('click', function () {
        let payload = 'name=' + encodeURIComponent(document.getElementById('value').value);

        const request = new XMLHttpRequest()
        request.open('POST', 'http://35.236.214.95:8080/cgi-bin/script.cgi', true)

        request.onload =  function () {
            let requestResult = parseInt(request.responseText.replace(/[^\d.]/g, ''));

            document.querySelector("#subtitle").textContent = "The result of checking the text for uniqueness";
            document.querySelector("#content").classList.add('hide');
            document.querySelector("#resultBlock").classList.remove('hide');

            if (isNaN(requestResult)) {
                document.querySelector("#resultTitle").innerHTML = `Text is not provided! Enter atleast 3 words`;
            } else {
                console.log(requestResult);
                document.querySelector("#resultTitle").innerHTML = `Your text is ${requestResult}% unique`;
            }
        }

        request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded')
        request.send(payload)
    })

    backButton.addEventListener('click', () => {
        document.querySelector("#subtitle").textContent = "Intelligent App for text processing";
        document.querySelector("#content").classList.remove('hide');
        document.querySelector("#resultBlock").classList.add('hide');
    })
})
