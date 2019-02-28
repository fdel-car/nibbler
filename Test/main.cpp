#include <dlfcn.h>
#include "IEntity.hpp"

void dlerror_wrapper(void) {
	std::cerr << "Error: " << dlerror()<< std::endl;
	exit(EXIT_FAILURE);
}

int main(int ac, char **av) {
	void *dl_handle;
	dl_handle = dlopen(av[1], RTLD_LAZY | RTLD_LOCAL);
	if (!dl_handle)
		dlerror_wrapper();
	IEntity	*(*PlayerCreator)(std::string);

	PlayerCreator = (IEntity *(*)(std::string)) dlsym(dl_handle, "createPlayer");
	if (!PlayerCreator)
		dlerror_wrapper();

	IEntity *newPlayer;
	newPlayer = PlayerCreator("Conan");

	std::cout << "Player score : " << newPlayer->getScore() << std::endl;
	newPlayer->announces();
	newPlayer->markPoint();
	newPlayer->announces();

	void (*PlayerDestructor)(IEntity *);

	PlayerDestructor = (void(*)(IEntity*)) dlsym(dl_handle, "deletePlayer");
	if (!PlayerDestructor)
		dlerror_wrapper();

	PlayerDestructor(newPlayer);

	dlclose(dl_handle);
	return 0;

}
