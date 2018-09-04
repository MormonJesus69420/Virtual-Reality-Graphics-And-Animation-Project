# Documentation
Documentation and general usage guide for project in STE6249 Virtual Reality Graphics And Animation Project.

## Getting Started
In order to run this project read following subheadings. In order to work on the project please read the section titled "Working Ethics and Style".

### Prerequisites
#### Libraries
 * GLEW can be found here: [GLEW Mainpage](http://glew.sourceforge.net/)

### Running this code
Follow the [setup guide](GMlibSetupGuide2018.pdf) to set up your project and get it running on your PC. After that, you can run the project in QtCreator.

## Working Ethics And Style
### Cloning project
Open GIT bash and execute following commands to clone project into your pc.
```bash
cd [PATH_TO_DESIRED_PROJECT_LOCATION]
git clone git@source.coderefinery.org:MormonJesus69420/STE6249-Virtual-Reality-Graphics-And-Animation-Project.git [PROJECT_FOLDER_NAME]
```

### Committing your changes
After staging your work you are ready to commit your work. Avoid using this way of committing work at ***all cost***. Use it only if you have made a really small changes to code which can be summed up in less than 50 characters.
```bash
git commit -m "[COMMIT_MESSAGE]"
```
For all other cases, which are 99% of cases you will need to run:
```bash
git commit
```
When writing a commit message follow these rules:
 1. Separate subject from body with a blank line
 2. Limit the subject line to 50 characters
 3. Capitalize the subject line
 4. Do not end the subject line with a period
 5. Use the imperative mood in the subject line
 6. Wrap the body at 72 characters
 7. Use the body to explain what and why vs. how

For example:
```
Summarize changes in around 50 characters or less

More detailed explanatory text, if necessary. Wrap it to about 72
characters or so. In some contexts, the first line is treated as the
subject of the commit and the rest of the text as the body. The
blank line separating the summary from the body is critical (unless
you omit the body entirely); various tools like `log`, `shortlog`
and `rebase` can get confused if you run the two together.

Explain the problem that this commit is solving. Focus on why you
are making this change as opposed to how (the code explains that).
Are there side effects or other unintuitive consequences of this
change? Here's the place to explain them.

Further paragraphs come after blank lines.

 * Bullet points are okay, too.
 * Typically a hyphen or asterisk is used for the bullet, preceded
   by a single space, with blank lines in between, but conventions
   vary here.

If you use an issue tracker, put references to them at the bottom,
like this:

Resolves: #123
See also: #456, #789
```

### Pushing project
After you have added and committed changes you are ready to push them upstream, well actually not. **ALWAYS REBASE BEFORE PUSHING!** You never know if somebody has pushed some changes since last time you have re-based your project. To do so run following commands:
```bash
git fetch
git rebase
```
This will fetch changes from the remote repository and re-base your project based on those. **Do not use `pull` or  `fetch` followed by `merge [REMOTE/BRANCH]`**

### Merge conflicts
It is possible that you will get some merge conflicts when you try to rebase your project or push it upstream. If that is the case GIT marks them for you in the files. You’ll see sections like this:
```c++
<<<<<<< HEAD
std::cout << "Hello there!" << std::endl;
=======
std::cout << "General Kenobi!" << std::endl;
>>>>>>> REMOTE/BRANCH
```
The first section, `HEAD` is what you have in your version. The second section, `REMOTE/BRANCH` is what GIT found in the version you were trying to merge into.  
You’ll have to decide what the file should contain, and you’ll need to edit it. After choosing what to do with these changes you will need to remove the marking put in place by GIT, stage and commit those changes.
```bash
git add [FILENAME]
git commit
```

### Finally pushing your project upstream
After all these steps you are ready to push your code to remote repository, this is simply done by running:
```bash
git push [REMOTE] [BRANCH]
```
If you have created new branch and want to push it you will need to append `-u` before specifying remote, so:
```bash
git push -u [REMOTE] [BRANCH]
```

## Authors
**Daniel Aaron Salwerowicz** - *Developer and memer* -
[CodeRefinery](https://source.coderefinery.org/MormonJesus69420)

**Christopher Kragebøl Hagerup** - *Developer and weeb* -
[CodeRefinery](https://source.coderefinery.org/Krahager)

## Acknowledgments
**Daniele Procida** - *Git commands and cheatsheet* -
[Webshite](http://dont-be-afraid-to-commit.readthedocs.io/en/latest/git/index.html)

**Chris Beams** - *How to Write a Git Commit Message* -
[Webshite](https://chris.beams.io/posts/git-commit/)
