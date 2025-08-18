#include "wad.h"
#include <filesystem>

using namespace std;
namespace WAD {
	// TODO: Tap into existing logic inside zandronum for this, the mod box is going to get glitchy but should work
	vector<string> wadPaths = { "./", "E:/zandronum/" };

	unordered_set<string> wadExtensions = { ".wad" };
	unordered_set<string> allExtensions = {
		".wad", ".pk3", ".pk7", ".zip", ".7z", ".bex", ".beh", ".deh", ".cld"
	};

	unordered_set<string> knownWads{
		"doom.wad", "doom2.wad", "heretic.wad", "hexen.wad",
		"zandronum.pk3", "skulltag_actors.pk3"
	};

	vector<string> find_impl(unordered_set<string> allowedExtensions, bool filter=false) {
		vector<string> out;

		for (auto path : wadPaths)
		{
			filesystem::path fspath{ path };
			if (filesystem::is_directory(fspath)) {
				for (const auto& entry : filesystem::directory_iterator(fspath)) {
					if (!entry.is_regular_file())
						continue;

					string ext = entry.path().extension().string();
					string filename = entry.path().filename().string();
					transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
					transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

					if (filter && knownWads.contains(filename)) {
						continue;
					}

					if (allowedExtensions.contains(ext)) {
						out.push_back(filename);
					}
				}
			}
		}

		return out;
	}

	vector<string> find_aux() {
		return find_impl(allExtensions, true);
	}

	vector<string> find_wads() {
		return find_impl(wadExtensions);
	}
}

