#version 450 core

layout(binding = 0) uniform sampler2D 	ColTexture;

out vec4 FragColor;	// Color that will be used for the fragment

in VS_DATA
{
	flat int alpha;
    vec2 uv;
} inData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	vec4 colour = texture( ColTexture, inData.uv );

	if( inData.alpha == 0 )
	{
		if( 1.0-colour.w > 0.0 )
		{
			float greyscale = 1.0-(colour.x*0.299 + colour.y*0.587 + colour.z*0.114);
			FragColor = vec4(greyscale);
		}
	}
	else
	{
		float greyscale = 1.0-(colour.x*0.299 + colour.y*0.587 + colour.z*0.114);
		FragColor = vec4(greyscale);
	}
		
}