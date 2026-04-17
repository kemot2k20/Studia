const capitalize = (str) => {
    return str.charAt(0).toUpperCase() + str.slice(1);
}  

const capitalizeSentence = (sentence) => {
    return sentence
      .split(" ")
      .map(word => capitalize(word))
      .join(" ");
}  

console.log(capitalizeSentence("alice"));
console.log(capitalizeSentence("alice in wonderland"));


// const ids = [];

// const generateId = () => {
//   let id = 0;

//   do {
//     id++;
//   } while (ids.includes(id));

//   ids.push(id);
//   return id;
// };

const ids = new Set();

const generateId = () => {
    let id = 0;
    do {
        id++
    } while (ids.has(id));

    ids.add(id);
    return id;
}

console.time("generateId");

for (let i = 0; i < 3000; i++) {
  generateId();
}

console.timeEnd("generateId");

// generateId: 5.236s

// po zmianie : 33.848ms

let library = [];

const addBookToLibrary = ({ title, author, pages, isAvailable, ratings }) => {
  if (typeof title !== "string") throw new Error("Invalid title1");
  if (title.length == 0) throw new Error("Invalid")
  if (typeof author !== "string" || author.length == 0) throw new Error("Invalid author");
  if (typeof pages !== "number" || pages < 0) throw new Error("Invalid pages");
  if (typeof isAvailable !== "boolean") throw new Error("Invalid availability");
  if (!Array.isArray(ratings)) throw new Error("Ratings must be an array");
  if (!ratings.every(r => typeof r === "number" && r >= 0 && r <= 5)) throw new Error("Invalid rating value");

  library.push({
    title,
    author,
    pages,
    isAvailable,
    ratings,
  });
}

const testAddingBooks = (tests) => {
    for (const { testCase, shouldFail } of tests) {
      const [title, author, pages, isAvailable, ratings] = testCase;
      try {
        addBookToLibrary({ title, author, pages, isAvailable, ratings });
        if (shouldFail) {
          console.log("❌ WA Test failed (should have thrown an error):", testCase);
        } else {
          console.log("✅ OK Test passed:", testCase);
        }
      } catch (e) {
        if (shouldFail) {
          console.log("✅ OK Test passed (error thrown as expected):", e.message);
        } else {
          console.log("❌ WA Test failed (unexpected error):", e.message);
        }
      }
    }
};  

const testCases = [
    { testCase: ["", "Author", 200, true, []], shouldFail: true },
    { testCase: ["Title", "", 200, true, []], shouldFail: true },
    { testCase: ["Title", "Author", -1, true, []], shouldFail: true },
    { testCase: ["Title", "Author", 200, "yes", []], shouldFail: true },
    { testCase: ["Title", "Author", 200, true, [1, 2, 3, 6]], shouldFail: true },
    {
      testCase: ["Title", "Author", 200, true, [1, 2, 3, "yes"]],
      shouldFail: true,
    },
    { testCase: ["Title", "Author", 200, true, [1, 2, 3, {}]], shouldFail: true },
    { testCase: ["Title", "Author", 200, true, []], shouldFail: false },
    { testCase: ["Title", "Author", 200, true, [1, 2, 3]], shouldFail: false },
    { testCase: ["Title", "Author", 200, true, [1, 2, 3, 4]], shouldFail: false },
    {
      testCase: ["Title", "Author", 200, true, [1, 2, 3, 4, 5]],
      shouldFail: false,
    },
    {
      testCase: ["Title", "Author", 200, true, [1, 2, 3, 4, 5]],
      shouldFail: false,
    },
];
  
testAddingBooks(testCases);

const addBooksToLibrary = (books) => {
    books.forEach(book => {
      const [title, author, pages, isAvailable, ratings] = book;
      addBookToLibrary({ title, author, pages, isAvailable, ratings });
    });
};

const books = [
    ["Alice in Wonderland", "Lewis Carroll", 200, true, [1, 2, 3]],
    ["1984", "George Orwell", 300, true, [4, 5]],
    ["The Great Gatsby", "F. Scott Fitzgerald", 150, true, [3, 4]],
    ["To Kill a Mockingbird", "Harper Lee", 250, true, [2, 3]],
    ["The Catcher in the Rye", "J.D. Salinger", 200, true, [1, 2]],
    ["The Hobbit", "J.R.R. Tolkien", 300, true, [4, 5]],
    ["Fahrenheit 451", "Ray Bradbury", 200, true, [3, 4]],
    ["Brave New World", "Aldous Huxley", 250, true, [2, 3]],
    ["The Alchemist", "Paulo Coelho", 200, true, [1, 2]],
    ["The Picture of Dorian Gray", "Oscar Wilde", 300, true, [4, 5]],
];
  
addBooksToLibrary(books);
console.log(library);