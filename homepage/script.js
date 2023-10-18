const submitBtn = document.querySelector('form');
let body = document.querySelector('body');

submitBtn.addEventListener('submit', greet);

function greet()
{
    let recipe = document.querySelector('input').value
    if (recipe == "Feta")
    {
        alert("Anything you can do. I can do feta.");
    }
    else if (recipe == "Ramen")
    {
        alert("Ramen best men.")
    }
    else
    {
        alert("Bless your lucky starch.")
    }
}
