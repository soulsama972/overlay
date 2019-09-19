#include "model11.hpp"

std::string stringPixelShader = std::string(""\
	"	struct PixelInputType"\
	"{"\
	"	float4 position : SV_POSITION;"\
	"	float4 color : COLOR;"\
	"};"\
	"float4 ColorPixelShader(PixelInputType input) : SV_TARGET"\
	"{"\
	"return input.color;"\
	"}");

std::string stringVertexShader = std::string(""\
	"cbuffer MatrixBuffer"\
	"{"\
	"	float2 screenPos;"\
	"};"
	"struct VertexInputType"\
	"{"\
	"	float4 position : POSITION;"\
	"   float2 instancePosition : TEXCOORD0;"\
	"	float2 instanceSize : TEXCOORD1; "\
	"	float4 color : COLOR;"\
	"};"\

	"struct PixelInputType"\
	"{"\
	"	float4 position : SV_POSITION;"\
	"	float4 color : COLOR;"\
	"};"\

	"PixelInputType ColorVertexShader(VertexInputType input)"\
	"{"\
	"	PixelInputType output;"\
	"	matrix scale ="\
	"	{"\
	"		{ input.instanceSize.x / screenPos.x, 0, 0, 0 },"\
	"		{ 0, input.instanceSize.y / screenPos.y, 0, 0 },"\
	"		{ 0, 0, 1, 0 },"\
	"		{ 0, 0, 0, 1 }"\
	"	};"\
	"	matrix translate ="\
	"	{"\
	"		{ 1, 0, 0, (input.instancePosition.x + input.instanceSize.x / 2) * 2 / screenPos.x -1 },"\
	"		{ 0, 1, 0, 1 - 2 * (input.instancePosition.y + input.instanceSize.y / 2) / screenPos.y  },"\
	"		{ 0, 0, 1, 0 },"\
	"		{ 0, 0, 0, 1 }"\
	"	};"\
	"input.position.w = 1.0f;"\
	"matrix world = mul(translate,scale);"\
	"output.position = mul(world,input.position);"\
	"output.color = input.color;"\
	""\
	"	return output;"\
	"}");





void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd)
{
	char* compileErrors;
	size_t bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", L"", MB_OK);
	return;
}