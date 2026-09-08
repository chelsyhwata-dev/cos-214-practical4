# TaskForge — COS 214 Practical 4

TaskForge is a hierarchical work processing system based on a software delivery pipeline. The project demonstrates how the **Composite, Iterator, State, and Decorator design patterns** can work together in one system.

### Team

* Chelsy Whata
* Yariv Singh
* Leensa Keterew

### Student Numbers

* u25657764
* u24612082
* u24577988

## Domain

TaskForge represents a software development workflow. An **Epic** can contain **Features**, and Features can contain **Stories** or individual **Tasks**. 
Each Task has its own state and moves through different stages:**Backlog → InProgress → InReview → Done**

A Task can also become **Blocked** when something prevents it from continuing.

We also use Decorators to add extra responsibilities to WorkItems when needed. These include:
* High-priority flagging
* Mandatory code review
* Security audit

## Requirements

The only thing that needs to be installed on the host computer is **Docker**. The Docker container already contains the other tools we need, such as:
* g++
* make
* gdb
* valgrind

## 1. Building the Docker Image

Open a terminal in the project folder where the `Dockerfile` is located and run:

```bash
docker build -t taskforge .
```

This creates the Docker image and builds the project.
If you only changed some source code, you can use the development method in Section 5 instead of rebuilding the whole image every time.

## 2. Running the Program

To run the program:

```bash
docker run --rm taskforge
```

The `main.cpp` file automatically runs the different test and demonstration scenarios.
It does not require any user input. It tests the Composite, Iterator, State and Decorator patterns, as well as some structural changes.
The program should finish quickly and exit with status `0`.

## 3. Using GDB

To open a terminal inside the Docker container:

```bash
docker run --rm -it taskforge bash
```

Then run GDB:

```bash
gdb ./taskforge
```

Example commands:

```text
(gdb) break Task::block
(gdb) run
(gdb) next
(gdb) print task->getName()
(gdb) print *this
(gdb) continue
(gdb) quit
```

The breakpoint can be changed depending on what function we want to investigate.

For example:

```text
WorkGroup::replaceChild -> we did this one for our evidence
SecurityAuditFlagDecorator::getEstimatedHours
```

We can use `next` or `step` to go through the code and `print` to check variables.
The GDB output can then be captured using a screenshot or copied into the PDF for the write-up.

## 4. Using Valgrind

Valgrind can also be run inside the container:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./taskforge
```

Or it can be run directly from the host:

```bash
docker run --rm taskforge valgrind --leak-check=full --show-leak-kinds=all ./taskforge
```

There is also a Makefile command:

```bash
make valgrind
```

## 6. Building Without Docker

Docker is the main environment for the project, but if we have g++ installed on Linux or WSL, we can also build the project locally.

```bash
make
```

To build and run:

```bash
make run
```

To remove the build files:

```bash
make clean
```

The project uses **C++11**.

## Project Structure

Practical_4
.
├── Dockerfile
├── Makefile
├── README.md
├── main.cpp
│
├── WorkItem.h / .cpp
├── WorkGroup.h / .cpp
├── Task.h / .cpp
├── TaskState.h / .cpp
│
├── WorkItemIterator.h / .cpp
├── PreOrderIterator.h / .cpp
├── BlockedWorkIterator.h / .cpp
├── NullIterator.h / .cpp
│
├── WorkItemDecorator.h / .cpp
├── HighPriorityDecorator.h / .cpp
├── CodeReviewRequiredDecorator.h / .cpp
├── SecurityAuditFlagDecorator.h / .cpp


### Main Pattern Classes

* **WorkItem** — common interface for the work items.
* **WorkGroup** — Composite that can contain other work items.
* **Task** — Leaf in the Composite pattern and also uses the State pattern.
* **TaskState** — Handles the different Task states.
* **PreOrderIterator** — Used to move through the whole work hierarchy.
* **BlockedWorkIterator** — Finds work items that are blocked.
* **NullIterator** — Used when there are no child items to iterate through.
* **WorkItemDecorator** — Base Decorator class.
* **HighPriorityDecorator** — Adds high-priority behaviour.
* **CodeReviewRequiredDecorator** — Adds a code review requirement.
* **SecurityAuditFlagDecorator** — Adds a security audit requirement.

## GitHub

**Repository:** https://github.com/chelsyhwata-dev/cos-214-practical4.git

We used GitHub to keep the project code together and allow all three team members to work on the project. We used branches to work on changes separately before adding them to the main project. Pull requests were used when changes needed to be reviewed and merged.
