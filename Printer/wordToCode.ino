void wordToCode(int code[], char singleWord){
  switch(singleWord){
    case 'a':
      code[0] = 1;
      code[1] = 0;
      code[2] = 0;
      code[3] = 0;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'b':
      code[0] = 1;
      code[1] = 0;
      code[2] = 1;
      code[3] = 0;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'c':
      code[0] = 1;
      code[1] = 1;
      code[2] = 0;
      code[3] = 0;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'd':
      code[0] = 1;
      code[1] = 1;
      code[2] = 0;
      code[3] = 1;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'e':
      code[0] = 1;
      code[1] = 0;
      code[2] = 0;
      code[3] = 1;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'f':
      code[0] = 1;
      code[1] = 1;
      code[2] = 1;
      code[3] = 0;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'g':
      code[0] = 1;
      code[1] = 1;
      code[2] = 1;
      code[3] = 1;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'h':
      code[0] = 1;
      code[1] = 0;
      code[2] = 1;
      code[3] = 1;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'i':
      code[0] = 0;
      code[1] = 1;
      code[2] = 1;
      code[3] = 0;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'j':
      code[0] = 0;
      code[1] = 1;
      code[2] = 1;
      code[3] = 1;
      code[4] = 0;
      code[5] = 0;
      break;
    case 'k':
      code[0] = 1;
      code[1] = 0;
      code[2] = 0;
      code[3] = 0;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'l':
      code[0] = 1;
      code[1] = 0;
      code[2] = 1;
      code[3] = 0;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'm':
      code[0] = 1;
      code[1] = 1;
      code[2] = 0;
      code[3] = 0;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'n':
      code[0] = 1;
      code[1] = 1;
      code[2] = 0;
      code[3] = 1;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'o':
      code[0] = 1;
      code[1] = 0;
      code[2] = 0;
      code[3] = 1;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'p':
      code[0] = 1;
      code[1] = 1;
      code[2] = 1;
      code[3] = 0;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'q':
      code[0] = 1;
      code[1] = 1;
      code[2] = 1;
      code[3] = 1;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'r':
      code[0] = 1;
      code[1] = 0;
      code[2] = 1;
      code[3] = 1;
      code[4] = 1;
      code[5] = 0;
      break;
    case 's':
      code[0] = 0;
      code[1] = 1;
      code[2] = 1;
      code[3] = 0;
      code[4] = 1;
      code[5] = 0;
      break;
    case 't':
      code[0] = 0;
      code[1] = 1;
      code[2] = 1;
      code[3] = 1;
      code[4] = 1;
      code[5] = 0;
      break;
    case 'u':
      code[0] = 1;
      code[1] = 0;
      code[2] = 0;
      code[3] = 0;
      code[4] = 1;
      code[5] = 1;
      break;
    case 'v':
      code[0] = 1;
      code[1] = 0;
      code[2] = 1;
      code[3] = 0;
      code[4] = 1;
      code[5] = 1;
      break;
    case 'w':
      code[0] = 0;
      code[1] = 1;
      code[2] = 1;
      code[3] = 1;
      code[4] = 0;
      code[5] = 1;
      break;
    case 'x':
      code[0] = 1;
      code[1] = 1;
      code[2] = 0;
      code[3] = 0;
      code[4] = 1;
      code[5] = 1;
      break;
    case 'y':
      code[0] = 1;
      code[1] = 1;
      code[2] = 0;
      code[3] = 1;
      code[4] = 1;
      code[5] = 1;
      break;
    case 'z':
      code[0] = 1;
      code[1] = 0;
      code[2] = 0;
      code[3] = 1;
      code[4] = 1;
      code[5] = 1;
      break;
    case ' ':
      code[0] = 9;
      code[1] = 9;
      code[2] = 9;
      code[3] = 9;
      code[4] = 9;
      code[5] = 9;
      break;
    default:
      break;
  }
}
