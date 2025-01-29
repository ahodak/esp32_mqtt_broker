#pragma once

#define SCRIPTS "\
// script.js\n\
function setNowDateTime() {\
var today = new Date();\
var dd = today.getDate();\
var mm = today.getMonth() + 1;\
var yyyy = today.getFullYear();\
var h = today.getHours();\
var m = today.getMinutes();\
var s = today.getSeconds();\
document.getElementById('day').value = dd;\
document.getElementById('month').value = mm;\
document.getElementById('year').value = yyyy;\
document.getElementById('hour').value = h;\
document.getElementById('minute').value = m;\
document.getElementById('second').value = s;\
}\n\
function saveFormData(pageToRecall) {\
var data = [];\
var inputs = document.getElementsByTagName('input');\
for (var i = 0; i < inputs.length; i++) {\
var input = inputs[i];\
if (input.type == 'text' || input.type == 'password') {\
data.push({key: input.id, value: input.value});\
}\
}\
console.log(data);\
var url = pageToRecall + '?';\
for (var j = 0; j < data.length; j++) {\
var param = data[j];\
url += param.key + '=' + param.value + '&';\
}\
document.location = url;\
}"
