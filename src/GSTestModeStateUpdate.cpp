#include "GSTestModeStateUpdate.h"
#include <SDL.h>
#include <SDL_image.h>
#include "StructsDef.h"

namespace test_mode {
	enum SUBSTATES {
		LOADING = 0,
		UPDATING,
		UNLOADING,
	};

	int subState = LOADING;
	int charSprite_resourceID = -1;
	int cactusSprite_resourceID = -1;
	const float SPEED_CHARACTER = 0.5f;

	void loadingAssets(ResourceManager& resource) {
		SDL_Renderer* renderer = resource.renderer;
		SpriteAssets& spritesAssets = *(resource.spritesAssets);

		// Cargo el Sprite principal.
		string filePath = "assets/img/char_sprite.png";
		SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
		SDL_Rect dest;
		dest.x = WIDTH >> 1;
		dest.y = HEIGHT >> 1;
		dest.w = 60;
		dest.h = 60;

		Sprite charSprite;
		charSprite.dest = dest;
		charSprite.texture = texture;
		spritesAssets.push_back(charSprite); // Se agrega al final del vector
		charSprite_resourceID = spritesAssets.size() - 1; // Entonces obtengo el indice del asset agregado, asi luego lo puedo usar o eliminar.

		// Cargo el Sprite principal.
		string cactusfilePath = "assets/img/char_cactus.png";
		SDL_Texture* cactusTexture = IMG_LoadTexture(renderer, cactusfilePath.c_str());
		SDL_Rect cactusDest;
		cactusDest.x = WIDTH >> 3;
		cactusDest.y = HEIGHT >> 4;
		cactusDest.w = 60;
		cactusDest.h = 60;

		Sprite cactusSprite;
		cactusSprite.dest = cactusDest;
		cactusSprite.texture = cactusTexture;
		spritesAssets.push_back(cactusSprite); // Se agrega al final del vector
		cactusSprite_resourceID = spritesAssets.size() - 1; // Entonces obtengo el indice del asset agregado, asi luego lo puedo usar o eliminar.		
	}

	void unLoadingAssets(ResourceManager& resource) {
		SpriteAssets& spritesAssets = *(resource.spritesAssets);

		//Libero la textura
		SDL_DestroyTexture(spritesAssets[charSprite_resourceID].texture);

		//Libero la textura
		SDL_DestroyTexture(spritesAssets[cactusSprite_resourceID].texture);

		// Remuevo el asset del vector y ya no sera pintado nunca mas.
		spritesAssets.erase(spritesAssets.begin() + cactusSprite_resourceID);

		// Remuevo el asset del vector y ya no sera pintado nunca mas.
		spritesAssets.erase(spritesAssets.begin() + charSprite_resourceID);
	}

	void updateGame(float delta, ResourceManager& resource) {
		InputState inputState = *resource.inputState;
		GameStages gameStages = *resource.gameStages;
		SpriteAssets& spriteAssets = *resource.spritesAssets;

		Sprite & character = spriteAssets[charSprite_resourceID];
		// Si presione cualquier tecla (arriba, abajo, izquierda
		if (inputState.down) {
			character.dest.y += (SPEED_CHARACTER * delta);
		} 

		if (inputState.up) {
			character.dest.y -= (SPEED_CHARACTER * delta);
		} 
		
		if (inputState.left) {
			character.dest.x -= (SPEED_CHARACTER * delta);
		} 
		
		if (inputState.right) {
			character.dest.x += (SPEED_CHARACTER * delta);
		} 

		Sprite& cactus = spriteAssets[cactusSprite_resourceID];
		
		//Chequeo si colisionan sus rectangulos
		SDL_Rect cactusRect = cactus.dest;
		SDL_Rect characterRect = character.dest;

		if (SDL_HasIntersection(&characterRect, &cactusRect)) {
			//Si colisiono con el cactus... pongo al Character principal en ROJO
			character.fx_color.r = 255;
			character.fx_color.g = 0;
			character.fx_color.b = 0;
		}
		else if (characterRect.x < 0 || (characterRect.x + characterRect.w) > WIDTH || characterRect.y < 0 || (characterRect.y + characterRect.h) > HEIGHT) {
			// Si el Character esta "saliendo" de la pantall lo pongo en Verde
			character.fx_color.r = 0;
			character.fx_color.g = 255;
			character.fx_color.b = 0;
		}
		else {
			// Sino lo vuelvo a dejar en "blanco"
			character.fx_color.r = 255;
			character.fx_color.g = 255;
			character.fx_color.b = 255;
		}
		
	}
}

using namespace test_mode;

void GSTestModeStateUpdate(float delta, ResourceManager& resource) {

	switch (subState)
	{
	case LOADING:
		loadingAssets(resource);
		subState = UPDATING;
		break;
	case UPDATING:
		updateGame(delta, resource);
		break;
	case UNLOADING:
		break;
	default:
		break;
	}

}