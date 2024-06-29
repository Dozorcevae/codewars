/*
255, 255, 255 --> "FFFFFF"
255, 255, 300 --> "FFFFFF"
0, 0, 0       --> "000000"
148, 0, 211   --> "9400D3"
*/
function rgb(r, g, b) {
  
  function ToHex(value){
    if (value < 0) value = 0;
    if (value > 255) value = 255;
    
    return ("0" + value.toString(16).toUpperCase()).slice(-2);
  }
  
  
    return ToHex(r) + ToHex(g) + ToHex(b);
}
