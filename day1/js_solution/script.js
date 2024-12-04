document.addEventListener('DOMContentLoaded', () => {
    const data = [];
    const list1 = [];
    const list2 = [];

    document.getElementById("data").addEventListener('change', (event) => {
        const file = event.target.files[0];
        if (file && file.name === "data.txt") {
            if (file) {
                let r = new FileReader();
                r.onload = () => {
                    let content = r.result;
                    content = content.trim();
                    data.push(...content.split(/\s+/));
                }
                r.readAsText(file);
            } else {
                alert("Please upload correct file");
            }
        }
    });
    for (let i = 0; i < data.length; i += 2) {
        list1.push(data[i]);
        list2.push(data[i + 1]);
    }
    console.log(list1);
    console.log(list2);
    console.log(data);
})