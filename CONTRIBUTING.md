# Minigotchi contributing guidelines

Hello everyone, whether you be from Hacktoberfest or just some random person who stumbled upon this repository. Be sure to follow our contributing guidelines! This project needs just as much help from the community as it does from the maintainer itself.

### How can I contribute?

**Remember if you have write access to this, please do not make any changes to the main branch unless it is 100% necessary! Push commits to the [development](https://github.com/Pwnagotchi-Unofficial/minigotchi/tree/development) branch instead and make a [pull request](https://github.com/Pwnagotchi-Unofficial/minigotchi/pulls)!**

1. Fork and clone the repository

`git clone https://github.com/<your username>/minigotchi.git`

2. Go into cloned folder

`cd minigotchi`

3. Create a branch

`git checkout -b feature/your-feature`

3. Make your changes!

4. Test

I recommend getting the hardware needed and making sure this works. Test using the Arduino IDE(Not the web editor), and make sure the appropriate board is selected. I am using the latest version, and so should you.

5. Commit and push

`git commit -m "Add your concise commit message"`

`git push origin feature/your-feature`

6. Open a pull request

Go to your forked repository on GitHub, switch to the branch you just pushed, and click on the "New Pull Request" button. Fill out the necessary information and submit the pull request.

### More ways to contribute to development

Don't worry if you can't code! There's other ways to help!

1. You can also suggest features by communicating with me, see _Communication_ section of the [wiki](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Communication). Or, you can make an issue and suggest a feature to me.

2. Donations may be given to me through Social Media as well, although I am yet to get a better way to donate money. Here's [how](https://github.com/Pwnagotchi-Unofficial/minigotchi/wiki/Communication#communication) you can get ahold of me.

# Code structure

Most of the code is object oriented. I essentially grouped all of the main functions into their own `.cpp` and `.h` files, then inclded those files in the main `.ino` file. I am trying to make this as easy to maintain as possible. It's easier on me and others if each part was separated and organized. Here's some more info on the [OOP(Object oriented programming)](https://en.wikipedia.org/wiki/Object-oriented_programming).
