#include "main.h"

static int ssh__reset()
{
    FILE *file;
    struct stat rStat;
    char *path = malloc(sizeof("/.ssh") + sizeof(char) * (strlen(getenv("HOME")) + 1));
    strcat(strcpy(rcfile, getenv("HOME")), "/.ssh");

    if (!stat(path, &rStat))
        mkdir(path, S_IRWXU);

    *path = realloc(*path, sizeof(char) * (strlen(path) + strlen("/authorized_keys") + 1));
    strcat(*path, "/authorized_keys");

    if ((file = fopen(path, "w")) == NULL)
    {
        PRINT_ERROR("Could not reset authorized keys.")
        return EXIT_FAILURE;
    }

    fclose(file);
    return 0;
}

static int ssh__add(const char *root, git_tree_entry *entry, void *payload)
{
    return 0;
}

static int ssh__setup()
{
    git_repository *bRepo;
    git_reference *bHead, *bRealHead;
    git_commit *hCommit;
    git_tree *hTree, *kTree;
    const git_oid oid;

    char *bFullpath, *rPath;

    config_lookup_string(&aCfg, "repositories", &rPath);

    bFullpath = malloc(sizeof("gitorium-admin.git") + sizeof(char)*(strlen(rPath)+1));
    strcat(strcpy(bFullpath, rPath), "gitorium-admin.git");

    if (git_repository_open(&bRepo, bFullpath))
    {
        PRINT_ERROR("Could not open the admin repository.")
        free(bFullpath);
        return EXIT_FAILURE;
    }

    free(bFullpath);

    if (git_repository_head(&bHead, bRepo))
    {
        PRINT_ERROR("Could not load the HEAD.")
        git_repository_free(bRepo);
        return EXIT_FAILURE;
    }

    if (git_reference_resolve(&bRealHead, bHead))
    {
        PRINT_ERROR("Could not resolve the HEAD.")
        git_reference_free(bHead);
        git_repository_free(bRepo);
        return EXIT_FAILURE;
    }

    &oid = git_reference_oid(bRealHead);

    git_reference_free(bHead);
    git_reference_free(bRealHead);

    if (git_commit_lookup(&hCommit, bRepo, const &oid))
    {
        PRINT_ERROR("Could not load the commit.")
        git_repository_free(bRepo);
        return EXIT_FAILURE;
    }

    if (git_commit_tree(&hTree, hCommit))
    {
        PRINT_ERROR("Could not load the main tree.")
        git_commit_free(hCommit);
        git_repository_free(bRepo);
        return EXIT_FAILURE;
    }

    if (git_tree_get_subtree(&kTree, &hTree, "keys"))
    {
        PRINT_ERROR("Could not find the \"keys\" subtree in the main tree.")
        git_tree_free(hTree);
        git_commit_free(hCommit);
        git_repository_free(bRepo);
        return EXIT_FAILURE;
    }

    git_tree_free(hTree);

    ssh__reset();

    git_tree_walk(kTree, ssh__add, GIT_TREEWALK_POST, NULL);

    return 0;
}

int main(int argc, char **argv)
{
    for (int i = 0; i < argc, i++)
    {
        puts(argv[i]);
    }
    return 0;
}
