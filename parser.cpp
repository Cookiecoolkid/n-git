#include "parser.h"
#include "commands.h"

void Parser::getArgs() {
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
}

void Parser::parse() {
    if (args.size() < 2) printUsageAndExit();

    Commands commands(UsefulApi::cwd());

    if (args[1] == "init") {
        if (args.size() != 2) printUsageAndExit();
        commands.init();
    } else if (args[1] == "status") {
        if (args.size() != 2) printUsageAndExit();
        commands.status();
    } else if (args[1] == "add") {
        if (args.size() != 3) printUsageAndExit();
        commands.add(args[2]);
    } else if (args[1] == "checkout") {
        if (args.size() != 3) printUsageAndExit();
        commands.checkout(args[2]);
    } else if (args[1] == "log") {
        if (args.size() != 2) printUsageAndExit();
        commands.log();
    } else if (args[1] == "commit") {
        if (args.size() == 2) std::cerr << "please enter a commit message." << std::endl;
        if (args.size() != 3) printUsageAndExit();
        commands.commit(args[2]);
    } else if (args[1] == "rm") {
        if (args.size() != 3) printUsageAndExit();
        commands.rm(args[2]);
    } else {
        printUsageAndExit();
    }
}

void Parser::printUsageAndExit() {
    std::cerr << "Usage: nit <command> [arguments]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Commands:" << std::endl;
    std::cerr << "  init                    Initialize a new Nit repository" << std::endl;
    std::cerr << "  status                  Show the working tree status" << std::endl;
    std::cerr << "  add <file>              Add file contents to the index" << std::endl;
    std::cerr << "  checkout <commit id>    Checkout a commit" << std::endl;
    std::cerr << "  log                     Show commit hash value and msg" << std::endl;
    std::cerr << "  commit <msg>            Record changes to the repository" << std::endl;
    std::cerr << "  rm <file>               Remove file from the working tree and index" << std::endl;
    std::exit(EXIT_FAILURE);
}