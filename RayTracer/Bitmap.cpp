#include "Bitmap.h"
#include "Properties.h"
#include <vector>

Bitmap::Bitmap() {
	file.open(Properties::get()->getName() + ".tga", std::ios::binary);
}

Bitmap::~Bitmap() {
	file.close();
}

void Bitmap::SetHeader(const int width, const int height)
{
	// entête fichier PPM
	//file << "P3\n" << width << " " << height << "\n255\n";
	// entête fichier TGA
	TGAHeader header;
	memset(&header, 0, sizeof(TGAHeader));
	header.imagetype = 2;			// indique couleurs en RGB
	header.bits = 24;
	header.width = width;
	header.height = height;
	header.descriptor = 1 << 5;		// bit 5 => origine coin haut-gauche (flip Y)
	file.write((const char*)&header, sizeof(TGAHeader));
}

void Bitmap::OutputColor(const int ir, const int ig, const int ib)
{
	// stockage PPM
	//file << ir << " " << ig << " " << ib << "\n";
	// stockage TGA (attention le TGA stocke les données en BGR)
	// notez que je converti les entiers en char
	// cela fonctionne tel quel car le CPU est little-endian (architecture x86/x86-64)
	// cad: l'octet de poids faible est stockée en premier en mémoire
	file.write((const char*)&ib, sizeof(uint8_t));
	file.write((const char*)&ig, sizeof(uint8_t));
	file.write((const char*)&ir, sizeof(uint8_t));
}

void Bitmap::save(const char* filename, FREE_IMAGE_FORMAT format, const int width, const int height, std::vector<std::vector<vec3>> pixels)
{
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);

	//Set pixel array
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			RGBQUAD color;
			color.rgbRed = pixels[i][j].x;
			color.rgbGreen = pixels[i][j].y;
			color.rgbBlue = pixels[i][j].z;

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	FreeImage_FlipVertical(bitmap);
	FreeImage_Save(format, bitmap, filename);
}