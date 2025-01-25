document.addEventListener('DOMContentLoaded', function(){
    let navLink = document.querySelectorAll('.nav-link');

    for(let i = 0; i < navLink.length; i++)
    {
        navLink[i].addEventListener('click', function(){
            let active = document.querySelector('.active');

            active.classList.remove('active');
            active.style.color = 'black';
            this.classList.add('active');
            this.style.color = 'red';
        });
    }

});

