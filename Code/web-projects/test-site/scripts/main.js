let myHeading = document.querySelector("h1");
let myButton = document.querySelector("button");

myHeading.textContent = "Hello world!";
document.querySelector("html").onclick = function () {
  alert("别戳我，我怕疼。");
};

let myImage = document.querySelector("img");

myImage.onclick = function () {
  let mySrc = myImage.getAttribute("src");
  if (mySrc === "images/lufei.jpg") {
    myImage.setAttribute("src", "images/l.png");
  } else {
    myImage.setAttribute("src", "images/lufei.jpg");
  }
};

function setUserName() {
  let myName = prompt("请输入你的名字。");
  if (!myName || myName === null) {
    setUserName();
  } else {
    localStorage.setItem("name", myName);
    myHeading.innerHTML = "Mozilla 酷毙了，" + myName;
  }
}

if (!localStorage.getItem("name")) {
  setUserName();
} else {
  let storedName = localStorage.getItem("name");
  myHeading.textContent = "Mozilla 酷毙了，" + storedName;
}

myButton.onclick = function () {
  setUserName();
};
