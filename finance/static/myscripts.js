const togPassword = document.querySelector("#togglePassword");
const newPassword = document.querySelector("#newPassword");

const toggleConfirm = document.querySelector("#toggleConfirm");
const newConfirmation = document.querySelector("#newConfirmation");


function togglePassword()
{
  // toggle the type attribute
  const type = newPassword.getAttribute("type") === "password" ? "text" : "password";
  newPassword.setAttribute("type", type);

  // toggle the icon
  togPassword.classList.toggle("bi-eye");
}

function toggleConfirmation()
{
  // toggle the type attribute
  const type = newConfirmation.getAttribute("type") === "password" ? "text" : "password";
  newConfirmation.setAttribute("type", type);

  // toggle the icon
  toggleConfirm.classList.toggle("bi-eye");
}

function showPassword()
{
  var x = document.getElementById("password");
  if (x.type === "password")
  {
    x.type = "text";
  }
  else
  {
    x.type = "password";
  }
}





