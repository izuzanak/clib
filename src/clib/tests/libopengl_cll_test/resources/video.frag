
uniform sampler2D texture;
uniform vec4 borders;

void main()
{
  vec4 color = texture2D(texture, gl_TexCoord[0].xy);

  float x = gl_FragCoord.x;
  float y = gl_FragCoord.y;
  
  if (x < borders.x || x > borders.y || y < borders.z || y > borders.w)
  {
    color.a *= 0.1;
  }

  gl_FragColor = color;

  //float a = texture2D(texture, gl_TexCoord[0].xy).a;
  //gl_FragColor = vec4(gl_Color.rgb, gl_Color.a*a);
}

