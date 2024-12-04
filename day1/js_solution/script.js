document.addEventListener('DOMContentLoaded', () => {
    const data = [];

    document.getElementById("data").addEventListener('change', (event) => {
        const file = event.target.files[0];
        if (file && file.name === "data.txt") {
            if (file) {
                let r = new FileReader();
                r.onload = () => {
                    let content = r.result;
                    content = content.trim();
                    data.push(...content.split(/\s+/));
                    processData(data);
                }
                r.readAsText(file);
            } else {
                alert("Please upload correct file");
            }
        }
    });
    console.log(data);
})

function processData(data) {
    const list1 = [];
    const list2 = [];
    
    divideAndSort(list1, list2, data);
    //calculateDistance(list1, list2);
    calculateSimilarity(list1, list2);
    
    console.log(list1);
    console.log(list2);
}

Array.prototype.peek = function() {
    return this[this.length - 1];
}

function calculateSimilarity (list1, list2) {
    let result = 0;
    for (let i = 0; i < list1.length; i++) {
        let count = 0;
        for (let j = 0; j < list2.length; j++) {
            if (list1[i] === list2[j]) {
                count++;
            }
        }
        result += list1[i] * count;
    }
    console.log(result);
}

function divideAndSort(list1, list2, data) {
    for (let i = 0; i < data.length; i += 2) {
        list1.push(data[i]);
        list2.push(data[i + 1]);
    }
    list1.sort((a, b) => a - b);
    list2.sort((a, b) => a - b);
}

function calculateDistance(list1, list2) {
    let result = 0;

    while (list1.length > 0 && list2.length > 0) {
        let val1 = list1.peek();
        let val2 = list2.peek();

        if (val1 > val2) {
            result += val1 - val2;
        } else if (val2 > val1) {
            result += val2 - val1;
        }
        list1.pop();
        list2.pop();
    }
    console.log(result);
}
